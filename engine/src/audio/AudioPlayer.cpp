#include "AudioPlayer.h"
#include "audio/AudioCache.h"
#include "audio/AudioDecoder.h"
#include "AudioDecoderManager.h"
#include <string.h>
#include <cassert>

namespace ocf {

AudioPlayer::AudioPlayer()
    : m_pAudioCache(nullptr)
    , m_alSource(0)
    , m_bufferIds{0}
    , m_rotateBufferThread(nullptr)
    , m_isRotateThreadExited(false)
    , m_currentTime(0.0f)
    , m_volume(1.0f)
    , m_loop(false)
    , m_isDestory(false)
    , m_removeByAudioEngine(false)
    , m_streamingSource(false)
{
}

AudioPlayer::~AudioPlayer()
{
    destroy();

    if (m_streamingSource) {
        alDeleteBuffers(QUEUEBUFFER_NUM, m_bufferIds);
    }
}

void AudioPlayer::destroy()
{
    if (m_isDestory) {
        return;
    }

    m_isDestory = true;

    if (m_streamingSource) {
        if (m_rotateBufferThread != nullptr) {
            while (!m_isRotateThreadExited) {
                m_sleepCondition.notify_one();
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }

            if (m_rotateBufferThread->joinable()) {
                m_rotateBufferThread->join();
            }

            delete m_rotateBufferThread;
            m_rotateBufferThread = nullptr;
        }
    }

    alSourceStop(m_alSource);
    alSourcei(m_alSource, AL_BUFFER, 0);

    m_removeByAudioEngine = true;
}

bool AudioPlayer::setLoop(bool loop)
{
    if (!m_isDestory) {
        m_loop = loop;
        return true;
    }
    return false;
}

void AudioPlayer::setCache(AudioCache* cache)
{
    m_pAudioCache = cache;
}

void AudioPlayer::rotateBufferThread(int offsetFrame)
{
    char* tmpBuffer = nullptr;
    auto& fullPath = m_pAudioCache->m_fileFullPath;
    AudioDecoder* decoder = AudioDecoderManager::createDecoder(fullPath);
    long long rotateSleepTime = static_cast<long long>(QUEUEBUFFER_TIME_STEP * 1000) / 2;
    do {
        if (decoder == nullptr || !decoder->open(fullPath)) {
            break;
        }

        uint32_t framesRead = 0;
        const uint32_t frameToRead = m_pAudioCache->m_queBufferFrames;
        const uint32_t bufferSize = decoder->framesToBytes(frameToRead);

        tmpBuffer = static_cast<char*>(malloc(bufferSize));
        memset(tmpBuffer, 0, bufferSize);

        if (offsetFrame != 0) {
            decoder->seek(offsetFrame);
        }

        ALint sourceState;
        ALint bufferProcessed = 0;
        bool needToExitThread = false;

        while (!m_isDestory) {
            alGetSourcei(m_alSource, AL_SOURCE_STATE, &sourceState);
            if (sourceState == AL_PLAYING) {
                alGetSourcei(m_alSource, AL_BUFFERS_PROCESSED, &bufferProcessed);
                while (bufferProcessed > 0) {
                    bufferProcessed--;

                    m_currentTime += QUEUEBUFFER_TIME_STEP;
                    if (m_currentTime > m_pAudioCache->m_duration) {
                        if (m_loop) {
                            m_currentTime = 0.0f;
                        }
                        else {
                            m_currentTime = m_pAudioCache->m_duration;
                        }
                    }

                    framesRead = decoder->readFixedFrames(frameToRead, tmpBuffer);

                    if (framesRead == 0) {
                        if (m_loop) {
                            decoder->seek(0);
                            framesRead = decoder->readFixedFrames(frameToRead, tmpBuffer);
                        }
                        else {
                            needToExitThread = true;
                            break;
                        }
                    }

                    ALuint bid;
                    alSourceUnqueueBuffers(m_alSource, 1, &bid);

                    alBufferData(bid, m_pAudioCache->m_format, tmpBuffer, decoder->framesToBytes(framesRead),
                        decoder->getSampleRate());
                    alSourceQueueBuffers(m_alSource, 1, &bid);
                }
            }
            else if (sourceState != AL_PAUSED) {
                ALint queued;

                alGetSourcei(m_alSource, AL_BUFFERS_QUEUED, &queued);
                if (queued == 0) {
                    needToExitThread = true;
                }
                else {
                    alSourcePlay(m_alSource);
                }
            }

            std::unique_lock<std::mutex> lock(m_sleepMutex);
            if (m_isDestory || needToExitThread) {
                break;
            }

            m_sleepCondition.wait_for(lock, std::chrono::milliseconds(rotateSleepTime));
        }

    } while (false);

    AudioDecoderManager::destoroyDecoder(decoder);
    free(tmpBuffer);
    m_isRotateThreadExited = true;
}

bool AudioPlayer::play()
{
    if (m_isDestory) {
        return false;
    }

    bool result = false;

    do {
        if (m_pAudioCache->m_state != AudioCache::State::Ready) {
            break;
        }

        alSourcei(m_alSource, AL_BUFFER, 0);
        alSourcef(m_alSource, AL_PITCH, 1.0f);
        alSourcef(m_alSource, AL_GAIN, m_volume);
        alSourcei(m_alSource, AL_LOOPING, AL_FALSE);

        if (m_pAudioCache->m_queBufferFrames == 0) {

        }
        else {
            alGenBuffers(QUEUEBUFFER_NUM, m_bufferIds);

            ALenum alError = alGetError();
            if (alError != AL_NO_ERROR) {
                break;
            }

            for (int index = 0; index < QUEUEBUFFER_NUM; index++) {
                alBufferData(m_bufferIds[index], m_pAudioCache->m_format, m_pAudioCache->m_queBuffers[index],
                    m_pAudioCache->m_queBufferSize[index], m_pAudioCache->m_sampleRate);
            }

            m_streamingSource = true;
        }

        {
            if (m_streamingSource) {
                alSourceQueueBuffers(m_alSource, QUEUEBUFFER_NUM, m_bufferIds);
                m_rotateBufferThread = new std::thread(&AudioPlayer::rotateBufferThread, this,
                    m_pAudioCache->m_queBufferFrames * QUEUEBUFFER_NUM + 1);
            }
            else {
                alSourcei(m_alSource, AL_BUFFER, m_pAudioCache->m_alBufferId);
            }

            alSourcePlay(m_alSource);
        }

        ALenum alError = alGetError();
        if (alError != AL_NO_ERROR) {
            break;
        }

        ALint state;
        alGetSourcei(m_alSource, AL_SOURCE_STATE, &state);
 
        assert(state == AL_PLAYING);

        result = true;

    } while (false);

    if (!result) {
        m_removeByAudioEngine = true;
    }

    return result;
}

} // namespace ocf
