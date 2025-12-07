#include "ocf/audio/AudioEngine.h"
#include "audio/AudioEngineImpl.h"

namespace ocf {

const int AudioEngine::AUDIO_ID_INVALID = -1;
const float AudioEngine::TIME_UNKNOWN = -1.0f;

std::unordered_map<AUDIO_ID, AudioEngine::AudioInfo> AudioEngine::m_audioIdInfoMap;
std::unordered_map<std::string, AUDIO_ID> AudioEngine::m_audioPathIdMap;
AudioEngineImpl* AudioEngine::m_pAudioEngineImpl = nullptr;

AudioEngine::AudioInfo::AudioInfo()
    : volume(1.0f), loop(false), duration(TIME_UNKNOWN)
{
}

AudioEngine::AudioInfo::~AudioInfo()
{
}

bool AudioEngine::lazyInit()
{
    if (m_pAudioEngineImpl == nullptr) {
        m_pAudioEngineImpl = new AudioEngineImpl();
        if (!m_pAudioEngineImpl->init()) {
            delete m_pAudioEngineImpl;
            m_pAudioEngineImpl = nullptr;
            return false;
        }
    }
    return true;
}

void AudioEngine::end()
{
    uncacheAll();

    delete m_pAudioEngineImpl;
    m_pAudioEngineImpl = nullptr;
}

void AudioEngine::setLoop(AUDIO_ID audioID, bool loop)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        m_pAudioEngineImpl->setLoop(audioID, loop);
        iter->second.loop = loop;
    }
}

bool AudioEngine::isLoop(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        return iter->second.loop;
    }

    return false;
}

void AudioEngine::setVolume(AUDIO_ID audioID, float volume)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        volume = std::min(volume, 1.0f);
        volume = std::max(volume, 0.0f);

        if (iter->second.volume != volume) {
            m_pAudioEngineImpl->setVolume(audioID, volume);
            iter->second.volume = volume;
        }
    }
}

float AudioEngine::getVolume(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        return iter->second.volume;
    }

    return false;
}

AUDIO_ID AudioEngine::play2d(std::string_view filename, bool loop, float volume)
{
    AUDIO_ID result = AudioEngine::AUDIO_ID_INVALID;

    if (!lazyInit()) {
        return 0;
    }

    volume = std::min(volume, 1.0f);
    volume = std::max(volume, 0.0f);

    result = m_pAudioEngineImpl->play2d(filename, loop, volume, 0.0f);
    if (result != AUDIO_ID_INVALID) {
        m_audioPathIdMap[filename.data()] = result;

        auto& audioInfo = m_audioIdInfoMap[result];
        audioInfo.volume = volume;
        audioInfo.loop = loop;
        audioInfo.filePath = filename;
    }

    return result;
}

void AudioEngine::stop(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        m_pAudioEngineImpl->stop(audioID);
    }
}

void AudioEngine::stopAll()
{
    if (m_pAudioEngineImpl == nullptr) {
        return;
    }

    m_pAudioEngineImpl->stopAll();

    m_audioIdInfoMap.clear();
}

void AudioEngine::pause(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        m_pAudioEngineImpl->pause(audioID);
    }
}

void AudioEngine::pauseAll()
{
    for (auto& audioInfo : m_audioIdInfoMap) {
        m_pAudioEngineImpl->pause(audioInfo.first);
    }
}

void AudioEngine::resume(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        m_pAudioEngineImpl->resume(audioID);
    }
}

void AudioEngine::resumeAll()
{
    for (auto& audioInfo : m_audioIdInfoMap) {
        m_pAudioEngineImpl->resume(audioInfo.first);
    }
}

void AudioEngine::uncache(std::string_view filePath)
{
    if (m_pAudioEngineImpl == nullptr) {
        return;
    }

    auto audioIdIter = m_audioPathIdMap.find(filePath.data());
    if (audioIdIter != m_audioPathIdMap.end()) {
        m_pAudioEngineImpl->stop(audioIdIter->second);
        m_audioIdInfoMap.erase(audioIdIter->second);
        m_audioPathIdMap.erase(audioIdIter);
    }

    m_pAudioEngineImpl->uncache(filePath);
}

void AudioEngine::uncacheAll()
{
    if (m_pAudioEngineImpl == nullptr) {
        return;
    }

    stopAll();
    m_pAudioEngineImpl->unchacheAll();
}

} // namespace ocf
