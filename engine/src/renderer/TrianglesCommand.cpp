#include "ocf/renderer/TrianglesCommand.h"
#include "ocf/renderer/Texture.h"
#define XXH_INLINE_ALL
#include "xxhash.h"

namespace ocf {

TrianglesCommand::TrianglesCommand()
    : m_materialID(0)
    , m_texture(nullptr)
{
    m_type = Type::TrianglesCommand;
}

TrianglesCommand::~TrianglesCommand()
{
}

void TrianglesCommand::init(float globalZOrder, Texture* texture, const Triangles& triangles,
                            const math::mat4& modelView)
{
    RenderCommand::init(globalZOrder, modelView);

    m_texture = texture;
    m_triangles = triangles;

    genarateMaterialID();
}

void TrianglesCommand::genarateMaterialID()
{
    if (m_texture == nullptr) {
        m_materialID = 0;
        return;
    }

    m_materialID = XXH32(m_texture, sizeof(Texture), 0);
}

} // namespace ocf
