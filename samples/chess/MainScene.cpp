#include "MainScene.h"
#include "Texture.h"
#include <ocf/2d/Sprite.h>
#include <ocf/renderer/Texture.h>

using namespace ocf;
using namespace ocf::math;

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
    genarateTexture();

    return true;
}

void MainScene::onEnter()
{

    Sprite* sprite = Sprite::createWithTexture(getWoodTexture(), Rect(0,0,256,256));
    addNode(sprite);
}

void MainScene::onExit()
{
}

void MainScene::process(float delta)
{
}

