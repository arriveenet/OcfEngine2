#include "AudioCache.h"

#include <vector>
#include <cassert>
#include <string.h>
#include "audio/AudioDecoderManager.h"
#include "audio/AudioDecoder.h"
#include <AL/alext.h>

#define PCMDATA_CACHEMAXSIZE 1048576

namespace ocf {

AudioCache::AudioCache()
    : m_format(AL_FORMAT_STEREO16)
    , m_sampleRate(0)
    , m_duration(0.0f)
    , m_totalFrames(0)
    , m_framesRead(0)
    , m_state(State::Inital)
    , m_alBufferId(static_cast<ALuint>(AL_INVALID))
    , m_queBuffers()
    , m_queBufferSize()
    , m_queBufferFrames(0)
{
}

AudioCache::~AudioCache()
{
    if (m_state == State::Ready) {
        if (m_alBufferId != static_cast<ALuint>(AL_INVALID) && alIsBuffer(m_alBufferId)) {
            alDeleteBuffers(1, &m_alBufferId);
            m_alBufferId = static_cast<ALuint>(AL_INVALID);
        }
    }

    if (m_queBufferFrames > 0) {
        for (int i = 0; i < QUEUEBUFFER_NUM; ++i) {
            free(m_queBuffers[i]);
        }
    }
}

void AudioCache::readDate()
{
    m_state = State::Loading;

    AudioDecoder* pAudioDecoder = AudioDecoderManager::createDecoder(m_fileFullPath);
    do {
        if (pAudioDecoder == nullptr || !pAudioDecoder->open(m_fileFullPath)) {
            break;
        }

        const uint32_t originalTotalFrames   = pAudioDecoder->getTotalFrames();
        const uint32_t sampleRate            = pAudioDecoder->getSampleRate();
        const uint32_t channelCount          = pAudioDecoder->getChannelCount();
        const AudioSourceFormat sourceFormat = pAudioDecoder->getSourceFormat();

        uint32_t totalFrames     = originalTotalFrames;
        uint32_t dataSize        = pAudioDecoder->framesToBytes(totalFrames);
        uint32_t remainingFrames = totalFrames;

        switch (sourceFormat) {
        case AudioSourceFormat::PCM_U8:
            m_format = channelCount > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
            break;
        case AudioSourceFormat::PCM_16:
            m_format = channelCount > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
            break;
        default:
            assert(false);
            break;
        }

        m_sampleRate  = static_cast<ALsizei>(sampleRate);
        m_duration    = 1.0f * static_cast<float>(totalFrames) / static_cast<float>(sampleRate);
        m_totalFrames = totalFrames;

        if (dataSize < PCMDATA_CACHEMAXSIZE) {
            uint32_t framesRead = 0;
            const uint32_t framesToReadOnce =
                std::min(totalFrames, static_cast<uint32_t>(static_cast<float>(sampleRate) * QUEUEBUFFER_TIME_STEP * QUEUEBUFFER_NUM));

            alGenBuffers(1, &m_alBufferId);
            ALenum alError = alGetError();
            if (alError != AL_NO_ERROR) {
                break;
            }

            std::vector<char> pcmBuffer(dataSize, 0);
            char* pPcmData = pcmBuffer.data();

            framesRead = pAudioDecoder->readFixedFrames((std::min)(framesToReadOnce, remainingFrames),
                pPcmData + pAudioDecoder->framesToBytes(m_framesRead));

            m_framesRead += framesRead;
            remainingFrames -= framesRead;

            uint32_t frames = 0;
            while (m_framesRead < originalTotalFrames)
            {
                frames = (std::min)(framesToReadOnce, remainingFrames);
                if (m_framesRead + frames > originalTotalFrames)
                {
                    frames = originalTotalFrames - m_framesRead;
                }
                framesRead = pAudioDecoder->read(frames, pPcmData + pAudioDecoder->framesToBytes(m_framesRead));
                if (framesRead == 0)
                    break;
                m_framesRead += framesRead;
                remainingFrames -= framesRead;
            }

            if (m_framesRead < originalTotalFrames)
            {
                memset(pPcmData + pAudioDecoder->framesToBytes(m_framesRead), 0x00,
                    pAudioDecoder->framesToBytes(totalFrames - m_framesRead));
            }

            alBufferData(m_alBufferId, m_format, pPcmData, static_cast<ALsizei>(dataSize), static_cast<ALsizei>(sampleRate));

            m_state = State::Ready;
        }
        else {
            m_queBufferFrames = static_cast<uint32_t>(static_cast<float>(sampleRate) * QUEUEBUFFER_TIME_STEP);
            if (m_queBufferFrames == 0) {
                break;
            }

            const uint32_t queBufferBytes = pAudioDecoder->framesToBytes(m_queBufferFrames);

            for (int index = 0; index < QUEUEBUFFER_NUM; ++index)
            {
                m_queBuffers[index] = static_cast<char*>(malloc(queBufferBytes));
                m_queBufferSize[index] = queBufferBytes;

                pAudioDecoder->readFixedFrames(m_queBufferFrames, m_queBuffers[index]);
            }

            m_state = State::Ready;
        }

    } while (false);

    AudioDecoderManager::destoroyDecoder(pAudioDecoder);
}

} // namespace ocf
