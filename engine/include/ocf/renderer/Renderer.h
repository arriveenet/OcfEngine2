#pragma once
#include "ocf/base/Types.h"
#include "ocf/renderer/backend/Driver.h"
#include <vector>

namespace ocf {

class RenderCommand;
class RenderQueue;
class TrianglesCommand;
class VertexBuffer;
class IndexBuffer;

namespace backend {
class Driver;
}

class Renderer {
public:
    static constexpr int VBO_SIZE = 0x10000;
    static constexpr int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;

    Renderer();
    ~Renderer();

    bool init();

    void addCommand(RenderCommand* command);

    void beginFrame();

    void endFrame();

    void clear();

    void clean();

    void draw();

    backend::Driver* getDriver() const { return m_driver; }

    uint32_t getDrawCallCount() const { return m_drawCallCount; }

    uint32_t getDrawVertexCount() const { return m_drawVertexCount; }

protected:
    void flush();
    void flush2D();
    void flush3D();
    void visitRenderQueue(RenderQueue& queue);
    void doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands);
    void processRenderCommand(RenderCommand* command);
    void trianglesVerticesAndIndices(TrianglesCommand* command, unsigned int vertexBufferOffset);
    void drawTrianglesCommand();

private:
    std::vector<RenderQueue> m_renderGroups;
    backend::Driver* m_driver;
    std::vector<TrianglesCommand*> m_trianglesCommands;

    uint32_t m_drawCallCount = 0;
    uint32_t m_drawVertexCount = 0;

    struct TriangleBatchToDraw {
        TrianglesCommand* command = nullptr;
        uint32_t indicesToDraw = 0;
        uint32_t offset = 0;
    };
    TriangleBatchToDraw* m_triangleBatchToDraw = nullptr;
    int m_triangleBatchToDrawSize = 256;
    backend::RenderPrimitiveHandle m_triangleRenderPrimitive;
    VertexBuffer* m_triangleVertexBuffer = nullptr;
    IndexBuffer* m_triangleIndexBuffer = nullptr;

    Vertex3fC3fT2f m_triangleVertices[VBO_SIZE];
    unsigned short m_triangleIndices[INDEX_VBO_SIZE];
    unsigned int m_triangleVertexCount = 0;
    unsigned int m_triangleIndexCount = 0;
};

} // namespace ocf
