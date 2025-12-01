#pragma once

#include "ocf/2d/Node2D.h"
#include "ocf/base/Types.h"
#include "ocf/renderer/CustomCommand.h"
#include "ocf/renderer/ProgramManager.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include <vector>

namespace ocf {

class DrawNode : public Node2D {
public:
    using PrimitiveType = backend::PrimitiveType;
    static constexpr size_t INITIAL_VERTEX_BUFFER_SIZE = 512;
    static constexpr size_t INITIAL_INDEX_BUFFER_SIZE = 1024;

    static DrawNode* create();

    DrawNode();
    virtual ~DrawNode();

    bool init() override;
    void clear();

    void setPointSize(float pointSize) { m_pointSize = pointSize; }
    float getPointSize() const { return m_pointSize; }

    void setLineWidth(float lineWidth) { m_lineWidth = lineWidth; }
    float getLineWidth() const { return m_lineWidth; }

    void drawFillRect(const math::vec2& origin, const math::vec2& destanation, const math::vec4& color);

    void draw(Renderer* renderer, const math::mat4& transform) override;

protected:
    void initRenderCommand(CustomCommand& cmd, ProgramType programType, PrimitiveType primitiveType);
    void updateBuffers(CustomCommand& cmd);
    void updateUniforms(const math::mat4& transform, CustomCommand& cmd);

    /**
      * @brief 凸ポリゴンかどうかを判定する
      * @param prev 前の頂点
      * @param curr 現在の頂点
      * @param next 次の頂点
      * @return 凸ポリゴンならtrue、そうでなければfalse
      */
    bool isConvex(const math::vec2& prev, const math::vec2& curr, const math::vec2& next);

    /**
      * @brief 点が三角形の内部にあるかどうかを判定する
      * @param p 判定する点
      * @param a 三角形の頂点A
      * @param b 三角形の頂点B
      * @param c 三角形の頂点C
      * @return 内部にあればtrue、そうでなければfalse
      */
    bool isPointInTriangle(const math::vec2& p,
                           const math::vec2& a,
                           const math::vec2& b,
                           const math::vec2& c);

    /**
     * @brief 三角形分割を行う
     * @param vertices 分割する頂点のリスト
     * @return 分割された三角形の頂点のリスト
     */
    std::vector<math::vec2> triangulate(const std::vector<math::vec2>& vertices);

private:
    void primitiveReserve(int indexCount, int vertexCount);

    void primitiveRect(const math::vec2& a, const math::vec2& c, const math::vec4& color);

protected:
    bool m_dirtyTriangle;

    std::vector<Vertex3fC4f> m_triangleBuffers;

    CustomCommand m_customCommandTriangle;

    std::vector<Vertex2fC4fT2f> m_vertexBuffer;
    std::vector<uint16_t> m_indexBuffer;

    int m_vertexBufferCount = 0;
    int m_indexBufferCount = 0;

    float m_pointSize;
    float m_lineWidth;
};

} // namespace ocf