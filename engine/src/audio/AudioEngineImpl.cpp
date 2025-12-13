#include "AudioEngineImpl.h"

#include "audio/AudioDecoderManager.h"
#include "platform/PlatformMacros.h"
#include "ocf/core/FileUtils.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace ocf {

static ALCdevice* s_pALDevice = nullptr;
static ALCcontext* s_pALContext = nullptr;

AudioEngineImpl::AudioEngineImpl()
    : m_alSources()
    , m_currentAudioID(0)
{
}

AudioEngineImpl::~AudioEngineImpl()
{
    for (auto&& player : m_audioPlayers) {
        player.second->destroy();
    }

    if (s_pALContext != nullptr) {
        alDeleteSources(AUDIO_SOURCE_MAX, m_alSources);

        m_audioCaches.clear();

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(s_pALContext);
        s_pALContext = nullptr;
    }

    if (s_pALDevice != nullptr) {
        alcCloseDevice(s_pALDevice);
        s_pALDevice = nullptr;
    }

    AudioDecoderManager::destroy();
}

bool AudioEngineImpl::init()
{
    bool result = false;
    do {
        s_pALDevice = alcOpenDevice(nullptr);

        if (s_pALDevice != nullptr) {
            s_pALContext = alcCreateContext(s_pALDevice, nullptr);
            alcMakeContextCurrent(s_pALContext);

            alGenSources(AUDIO_SOURCE_MAX, m_alSources);
            ALenum alError = alGetError();
            if (alError != AL_NO_ERROR) {
                break;
            }

            for (int i = 0; i < AUDIO_SOURCE_MAX; i++) {
                m_unusedSourcesPool.push(m_alSources[i]);
            }

            AudioDecoderManager::init();

            const char* vender = alGetString(AL_VENDOR);
            const char* version = alGetString(AL_VERSION);

            OCF_LOG_INFO("OpenAL initialization successful. vender: {}, version: {}", vender, version);

            result = true;
        }
        else {
            OCF_LOG_INFO("OpenAL initialization failed. alcOpenDevice() failed.");
            break;
        }
    } while (false);

    return result;
}

void AudioEngineImpl::setVolume(AUDIO_ID audioID, float volume)
{
    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;

        alSourcef(player->m_alSource, AL_GAIN, volume);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            OCF_LOG_ERROR("%s: audio id = , error = %xn", __FUNCTION__, audioID, error);
        }
    }
}

void AudioEngineImpl::setLoop(AUDIO_ID audioID, bool loop)
{
    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;

        if (player->m_streamingSource) {
            player->setLoop(loop);
        }
        else {
            alSourcei(player->m_alSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        }

        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            OCF_LOG_ERROR("{}: audio id = {}, error = {}", __FUNCTION__, audioID, error);
        }
    }
}

AUDIO_ID AudioEngineImpl::play2d(std::string_view fileFullPath, bool loop, float volume, float time)
{
    if (s_pALDevice == nullptr) {
        return AudioEngine::AUDIO_ID_INVALID;
    }

    ALuint alSource = findSource();
    if (alSource == AL_INVALID) {
        return AudioEngine::AUDIO_ID_INVALID;
    }

    AudioPlayer* pAudioPlayer = new AudioPlayer();

    pAudioPlayer->m_alSource = alSource;
    pAudioPlayer->m_loop = loop;
    pAudioPlayer->m_volume = volume;
    if (time > 0.0f) {
        pAudioPlayer->m_currentTime = time;
    }

    AudioCache* pAudioCache = preload(fileFullPath);
    if (pAudioCache == nullptr) {
        delete pAudioPlayer;
        return AudioEngine::AUDIO_ID_INVALID;
    }

    pAudioPlayer->setCache(pAudioCache);
    m_audioPlayers.emplace(++m_currentAudioID, pAudioPlayer);

    pAudioPlayer->play();

    return m_currentAudioID;
}

void AudioEngineImpl::stop(AUDIO_ID audioID)
{
    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;
        player->destroy();
    }
}

void AudioEngineImpl::stopAll()
{
    for (auto&& player : m_audioPlayers) {
        player.second->destroy();
    }

    updatePlayers(true);
}

bool AudioEngineImpl::pause(AUDIO_ID audioID)
{
    bool result = false;

    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;

        alSourcePause(player->m_alSource);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            OCF_LOG_ERROR("{}: audio id = {}, error = {}", __FUNCTION__, audioID, error);
            return false;
        }

        result = true;
    }

    return result;
}

bool AudioEngineImpl::resume(AUDIO_ID audioID)
{
    bool result = false;

    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;

        alSourcePlay(player->m_alSource);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            OCF_LOG_ERROR("{}: audio id = {}, error = {}", __FUNCTION__, audioID, error);
            return false;
        }

        result = true;
    }

    return result;
}

AudioCache* AudioEngineImpl::preload(std::string_view filePath)
{
    AudioCache* pAudioCach = nullptr;

    auto iter = m_audioCaches.find(filePath.data());
    if (iter == m_audioCaches.end()) {
        pAudioCach = new AudioCache();
        m_audioCaches.emplace(filePath.data(), std::unique_ptr<AudioCache>(pAudioCach));

        pAudioCach->m_fileFullPath = FileUtils::getInstance()->fullPathForFilename(filePath.data());

        pAudioCach->readDate();
    }
    else {
        pAudioCach = iter->second.get();
    }

    return pAudioCach;
}

void AudioEngineImpl::updatePlayers(bool /* forStop */)
{
    AudioPlayer* player;
    ALuint alSource;

    for (auto iter = m_audioPlayers.begin(); iter != m_audioPlayers.end();) {
        player = iter->second;
        alSource = player->m_alSource;

        if (player->m_removeByAudioEngine) {

            iter = m_audioPlayers.erase(iter);
            delete player;
            m_unusedSourcesPool.push(alSource);
        }
        else {
            ++iter;
        }
    }
}

void AudioEngineImpl::uncache(std::string_view filePath)
{
    m_audioCaches.erase(filePath.data());
}

void AudioEngineImpl::unchacheAll()
{
    for (auto&& player : m_audioPlayers) {
        player.second->setCache(nullptr);
    }

    m_audioPlayers.clear();
}

ALuint AudioEngineImpl::findSource()
{
    ALuint sourceId = static_cast<ALuint>(AL_INVALID);
    if (!m_unusedSourcesPool.empty()) {
        sourceId = m_unusedSourcesPool.front();
        m_unusedSourcesPool.pop();
    }

    return sourceId;
}

} // namespace ocf
