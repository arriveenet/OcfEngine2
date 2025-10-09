#include "MainScene.h"
#include <ocf/2d/Sprite.h>
#include <ocf/base/Engine.h>
#include <ocf/math/vec3.h>
#include <ocf/math/mat4.h>
#include <ocf/math/matrix_transform.h>
#include <ocf/renderer/VertexBuffer.h>
#include <ocf/renderer/IndexBuffer.h>
#include <ocf/renderer/ProgramManager.h>
#include <ocf/renderer/Renderer.h>
#include <ocf/renderer/Material.h>
#include <ocf/renderer/Texture.h>
#include <ocf/renderer/TextureSampler.h>
#include <ocf/renderer/TrianglesCommand.h>

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

    Sprite* sprite = Sprite::createWithTexture(m_texture, Rect(0, 0, 2, 2));
    sprite->setPosition(vec2(100.0f, 100.0f));
    sprite->setSize(vec2(50.0f, 50.0f));
    addNode(sprite);
}

void MainScene::onExit()
{
}

