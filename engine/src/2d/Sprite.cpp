#include "ocf/2d/Sprite.h"

#include "platform/PlatformMacros.h"

#include "ocf/base/Camera.h"
#include "ocf/base/Engine.h"
#include "ocf/renderer/Renderer.h"
#include "ocf/renderer/Program.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/Material.h"
#include "ocf/renderer/TextureManager.h"

namespace ocf {

using namespace math;

Sprite* Sprite::create()
{
    Sprite* sprite = new Sprite();
    if (sprite->init()) {
        return sprite;
    }
    OCF_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::create(std::string_view filename)
{
    Sprite* sprite = new Sprite();
    if (sprite->initWithFile(filename)) {
        return sprite;
    }
    OCF_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite* Sprite::createWithTexture(const Ref<Texture>& texture, const math::Rect& rect)
{
    Sprite* sprite = new Sprite();
    if (sprite->initWithTexture(texture, rect)) {
        return sprite;
    }
    OCF_SAFE_DELETE(sprite);
    return nullptr;
}

Sprite::Sprite()
    : m_quad{}
    , m_isDirty(true)
    , m_rect(0, 0, 0, 0)
    , m_modelView(1.0f)
    , m_flippedX(false)
    , m_flippedY(false)
    , m_material(nullptr)
    , m_blendFunc(BlendFunc::DISABLE)
{
}

Sprite::~Sprite()
{
    OCF_SAFE_DELETE(m_material);
}

bool Sprite::init()
{
    return initWithTexture(nullptr, Rect(0, 0, 0, 0));
}

bool Sprite::initWithFile(std::string_view filename)
{
    Texture* texture = Engine::getInstance()->getTextureManager()->addImage(filename);
    if (texture != nullptr) {
        Rect rect;
        rect.m_size.x = static_cast<float>(texture->getWidth());
        rect.m_size.y = static_cast<float>(texture->getHeight());
        return initWithTexture(texture, rect);
    }

    return false;
}

bool Sprite::initWithTexture(const Ref<Texture>& texture, const math::Rect& rect)
{
    bool result = false;
    if (Node::init()) {
        m_flippedX = m_flippedY = false;

        setAnchorPoint(vec2(0.5f, 0.5f));

        m_quad.topLeft.color = {1.0f, 1.0f, 1.0f};
        m_quad.bottomLeft.color = {1.0f, 1.0f, 1.0f};
        m_quad.topRight.color = {1.0f, 1.0f, 1.0f};
        m_quad.bottomRight.color = {1.0f, 1.0f, 1.0f};

        m_rect = rect;

        setTexture(texture);
        setTextureRect(rect, rect.m_size);

        Program* program = ProgramManager::getInstance()->getBuiltinProgram(ProgramType::Basic);
        m_material = Material::create(program, m_texture.ptr());

        RenderCommand::PipelineState& pipeline = m_trianglesCommand.getPipelineState();
        pipeline.primitiveType = RenderCommand::PrimitiveType::TRIANGLES;
        pipeline.program = program->getHandle();
        pipeline.texture = m_texture->getHandle();

        TextureSampler sampler(TextureSampler::MinFilter::NEAREST,
                               TextureSampler::MagFilter::NEAREST);
        m_material->setParameter("uTexture", m_texture.ptr(), sampler);

        result = true;
    }

    return result;
}

void Sprite::setSize(const math::vec2& size)
{
    Node2D::setSize(size);
    updatePolygon();

    m_isDirty = true;
}

void Sprite::draw(Renderer* renderer, const math::mat4& transform)
{
    setMVPMarixUniform();

    m_trianglesCommand.init(m_globalZOrder, m_texture.ptr(), m_blendFunc, m_triangles, transform);

    renderer->addCommand(&m_trianglesCommand);
}

void Sprite::setTexture(const Ref<Texture>& texture)
{
    if (texture.ptr() == nullptr) {
        m_texture = Engine::getInstance()->getTextureManager()->getWhiteTexture();
        return;
    }

    m_texture = texture;
}

void Sprite::setFlippedX(bool flippedX)
{
    if (m_flippedX != flippedX) {
        m_flippedX = flippedX;
        flipX();
    }
}

void Sprite::setFlippedY(bool flippedY)
{
    if (m_flippedY != flippedY) {
        m_flippedY = flippedY;
        flipY();
    }
}

bool Sprite::isFlippedX() const
{
    return m_flippedX;
}

bool Sprite::isFlippedY() const
{
    return m_flippedY;
}

void Sprite::updatePolygon()
{
    static unsigned short indices[] = {0, 1, 2, 3, 2, 1};

    Rect copyRect;
    copyRect.m_position = vec2(0.0f, 0.0f);
    copyRect.m_size = m_size;

    setTextureCoords(m_rect, m_quad);
    setVertexCoords(copyRect, m_quad);

    m_triangles.vertices = &m_quad.topLeft;
    m_triangles.vertexCount = 4;
    m_triangles.indices = indices;
    m_triangles.indexCount = 6;
}

void Sprite::setTextureRect(const math::Rect& rect, const math::vec2& size)
{
    Node2D::setSize(size);

    setVertexRect(rect);
    updatePolygon();
}

void Sprite::setVertexRect(const math::Rect& rect)
{
    m_rect = rect;
}

void Sprite::setTextureCoords(const math::Rect& rectInPoints, QuadV3fC3fT2f& outQuad)
{
    if (m_texture == nullptr) {
        return;
    }

    const float atlasWidth = static_cast<float>(m_texture->getWidth());
    const float atlasHeight = static_cast<float>(m_texture->getHeight());

    float left = rectInPoints.m_position.x / atlasWidth;
    float bottom = rectInPoints.m_position.y / atlasHeight;
    float right = (rectInPoints.m_position.x + rectInPoints.m_size.x) / atlasWidth;
    float top = (rectInPoints.m_position.y + rectInPoints.m_size.y) / atlasHeight;

    outQuad.bottomLeft.texCoord = {left, bottom};
    outQuad.bottomRight.texCoord = {right, bottom};
    outQuad.topLeft.texCoord = {left, top};
    outQuad.topRight.texCoord = {right, top};
}

void Sprite::setVertexCoords(const math::Rect& rect, QuadV3fC3fT2f& outQuad)
{
    const float x1 = 0.0f;
    const float y1 = 0.0f;
    const float x2 = x1 + rect.m_size.x;
    const float y2 = y1 + rect.m_size.y;

    outQuad.bottomLeft.position = {x1, y1, 0.0f};
    outQuad.bottomRight.position = {x2, y1, 0.0f};
    outQuad.topLeft.position = {x1, y2, 0.0f};
    outQuad.topRight.position = {x2, y2, 0.0f};
}

void Sprite::flipX()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.topRight.texCoord);
    std::swap(m_quad.bottomLeft.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::flipY()
{
    std::swap(m_quad.topLeft.texCoord, m_quad.bottomLeft.texCoord);
    std::swap(m_quad.topRight.texCoord, m_quad.bottomRight.texCoord);
}

void Sprite::setMVPMarixUniform()
{
    Camera* camera = Camera::getVisitingCamera();
    m_material->setParameter("uMVPMatrix", &camera->getViewProjectionMatrix(), sizeof(mat4));

    RenderCommand::PipelineState& pipeline = m_trianglesCommand.getPipelineState();
    pipeline.uniforms = m_material->getUniformInfoMap();
    pipeline.uniformData = m_material->getUniformBuffer();
}

} // namespace ocf
