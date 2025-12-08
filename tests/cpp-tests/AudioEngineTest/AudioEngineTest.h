#pragma once
#include <ocf/audio/AudioEngine.h>
#include <ocf/base/Scene.h>

class AudioEngineTest : public ocf::Scene {
public:
    AudioEngineTest();
    ~AudioEngineTest() override;
    void onEnter() override;
    void onExit() override;

private:
    ocf::AUDIO_ID m_playingID = 0;
};
