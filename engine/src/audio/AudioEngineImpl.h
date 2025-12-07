#pragma once
#include <map>
#include <unordered_map>
#include <memory>
#include <queue>
#include "ocf/audio/AudioEngine.h"
#include "audio/AudioPlayer.h"
#include "audio/AudioCache.h"

#define AUDIO_SOURCE_MAX	32

namespace ocf {

class AudioEngineImpl {
public:
    AudioEngineImpl();
    ~AudioEngineImpl();

    bool init();
    void setVolume(AUDIO_ID audioID, float volume);
    void setLoop(AUDIO_ID audioID, bool loop);
    AUDIO_ID play2d(std::string_view fileFullPath, bool loop, float volume, float time);
    void stop(AUDIO_ID audioID);
    void stopAll();
    bool pause(AUDIO_ID audioID);
    bool resume(AUDIO_ID audioID);

    AudioCache* preload(std::string_view filePath);

    void updatePlayers(bool forStop);

    void uncache(std::string_view filePath);
    void unchacheAll();

private:
    ALuint findSource();

    ALuint m_alSources[AUDIO_SOURCE_MAX];
    std::queue<ALuint> m_unusedSourcesPool;
    std::map<std::string, std::unique_ptr<AudioCache>> m_audioCaches;
    std::unordered_map<AUDIO_ID, AudioPlayer*> m_audioPlayers;

    AUDIO_ID m_currentAudioID;
};

} // namespace ocf