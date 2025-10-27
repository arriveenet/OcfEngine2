#include "MainScene.h"
#include <ocf/2d/Label.h>
#include <ocf/2d/Sprite.h>
#include <ocf/3d/MeshInstance3D.h>
#include <ocf/base/Engine.h>
#include <ocf/base/Camera.h>
#include <ocf/base/View.h>

using namespace ocf;
using namespace ocf::math;

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::onEnter()
{
    unsigned char pixels[] = {0xff, 0x00, 0x00, 0x00, 0xff, 0x00,
                              0x00, 0x00, 0xff, 0xff, 0xff, 0x00};

    Texture::PixelBufferDescriptor buffer(pixels, sizeof(pixels), Texture::Format::RGB,
                                          Texture::Type::UNSIGNED_BYTE, nullptr);
    m_texture = Texture::create(Texture::Sampler::SAMPLER_2D, 2, 2, 1, 
                                Texture::InternalFormat::RGB8);
    m_texture->setImage(0, std::move(buffer));

    Camera* camera = Camera::createOrthographic(0.0f, 256.0f, 240.0f, 0.0f);

    View* view = new View();
    view->setCamera(camera);
    addNode(view);

    Sprite* sprite = Sprite::create("textures/pngtest.png");
    sprite->setPosition(vec2(100.0f, 30.0f));
    view->addChild(sprite);

    Sprite* textureSprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 2, 2));
    textureSprite->setPosition(vec2(100.0f, 100.0f));
    textureSprite->setSize(vec2(50.0f, 50.0f));
    addNode(textureSprite);


    Label* label = Label::create("Hello World!");
    label->setPosition(vec2(500.0f, 500.0f));
    addNode(label);

    MeshInstance3D* meshInstance = MeshInstance3D::create("models/teapot.obj");
    addNode(meshInstance);
}

void MainScene::onExit()
{
}

