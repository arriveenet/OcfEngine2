#include "AudioEngineTest.h"
#include <ocf/base/Engine.h>
#include <ocf/ui/UIButton.h>
#include "../MainScene.h"

using namespace ocf;

AudioEngineTest::AudioEngineTest()
{
}

AudioEngineTest::~AudioEngineTest()
{
}

void AudioEngineTest::onEnter()
{
    ui::Button* playButton = ui::Button::create("Play");
    playButton->setPosition({200.0f, 300.0f});
    playButton->setOnAction([this]() {
        AudioEngine::stop(m_playingID);
        m_playingID = AudioEngine::play2d("audio/タイフーンパレード.wav", false, 0.1f);
    });
    addNode(playButton);

    ui::Button* stopButton = ui::Button::create("Stop");
    stopButton->setPosition({400.0f, 300.0f});
    stopButton->setOnAction([this]() {
        AudioEngine::stop(m_playingID); 
    });
    addNode(stopButton);

    ui::Button* pauseButton = ui::Button::create("Pause");
    pauseButton->setPosition({600.0f, 300.0f});
    pauseButton->setOnAction([this]() {
        AudioEngine::pause(m_playingID); 
    });
    addNode(pauseButton);

    ui::Button* resumeButton = ui::Button::create("Resume");
    resumeButton->setPosition({800.0f, 300.0f});
    resumeButton->setOnAction([this]() {
        AudioEngine::resume(m_playingID); 
    });
    addNode(resumeButton);

    ui::Button* backButton = ui::Button::create("Back");
    backButton->setPosition({100.0f, 700.0f});
    backButton->setOnAction([this]() {
        auto scene = new MainScene();
        scene->init();
        Engine::getInstance()->replaceScene(scene);
    });
    addNode(backButton);
}

void AudioEngineTest::onExit()
{
    AudioEngine::stop(m_playingID);
}
