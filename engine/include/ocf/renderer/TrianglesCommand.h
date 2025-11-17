#pragma once
#include "ocf/base/Types.h"
#include "ocf/renderer/RenderCommand.h"

namespace ocf {

class Texture;

class TrianglesCommand : public RenderCommand {
public:
    struct Triangles {
        Triangles(Vertex3fC3fT2f* _vertices, unsigned short* _indices, unsigned int _vertexCount,
                  unsigned int _indexCount)
            : vertices(_vertices)
            , indices(_indices)
            , vertexCount(_vertexCount)
            , indexCount(_indexCount)
        {
        }

        Triangles() {}

        Vertex3fC3fT2f* vertices = nullptr;
        unsigned short* indices = nullptr;
        unsigned int vertexCount = 0;
        unsigned int indexCount = 0;
    };

    TrianglesCommand();
    ~TrianglesCommand();

    void init(float globalZOrder, Texture* texture, const BlendFunc& blendFunc,
              const Triangles& triangles, const math::mat4& modelView);

    uint32_t getMaterialID() const { return m_materialID; }
    const Triangles& getTriangles() const { return m_triangles; }
    unsigned int getVertexCount() const { return m_triangles.vertexCount; }
    unsigned int getIndexCount() const { return m_triangles.indexCount; }
    const Vertex3fC3fT2f* getVertices() const { return m_triangles.vertices; }
    const unsigned short* getIndices() const { return m_triangles.indices; }
    Texture* getTexture() const { return m_texture; }
    BlendFunc getBlendFunc() const { return m_blendFunc; }

protected:
    void generateMaterialID();

    uint32_t m_materialID;

    Triangles m_triangles;
    Texture* m_texture;
    BlendFunc m_blendFunc = BlendFunc::DISABLE;
};

} // namespace ocf
