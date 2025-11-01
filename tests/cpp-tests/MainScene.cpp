#include "MainScene.h"
#include <ocf/2d/Label.h>
#include <ocf/2d/Sprite.h>
#include <ocf/3d/MeshInstance3D.h>
#include <ocf/base/Engine.h>
#include <ocf/base/Camera.h>
#include <ocf/base/View.h>
#include <ocf/math/geometric.h>

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

    Camera* camera = Camera::createPerspective(math::radians(60.0f), 920.0f / 720.0f, 0.1f, 100.0f);
    camera->setPosition(vec3(8.0f, 5.0f, 8.0f));
    camera->setCenter(vec3(0.0f, 0.0f, 0.0f));

    View* view = new View();
    view->setCamera(camera);
    addNode(view);

    Sprite* textureSprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 2, 2));
    textureSprite->setPosition(vec2(100.0f, 100.0f));
    textureSprite->setSize(vec2(50.0f, 50.0f));
    addNode(textureSprite);

    m_meshInstance = MeshInstance3D::create("models/teapot.obj");
    view->addChild(m_meshInstance);
}

void MainScene::onExit()
{
}

void MainScene::process(float delta)
{
    auto rotateY = m_meshInstance->getRotation().y;
    m_meshInstance->setRotation(vec3(0.0f, rotateY + 20.0f * delta, 0.0f));
}

