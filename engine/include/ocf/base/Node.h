/* SPDX - License - Identifier : MIT */
#pragma once
#include "ocf/base/Object.h"
#include "ocf/math/mat4.h"

#include <algorithm>
#include <string>
#include <vector>

namespace ocf {

class Renderer;

class Node : public Object {
public:
    enum {
        FLAGS_TRANSFORM_DIRTY       = (1 << 0),
        FLAGS_CONTENT_SIZE_DIRTY    = (1 << 1),
        FLAGS_RENDER_AS_3D          = (1 << 3),

        FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
    };

    Node();
    virtual ~Node();

    virtual bool init();

    virtual void update(float deltaTime);

    virtual void updateNode(float deltaTime);

    virtual void draw(Renderer* renderer, const math::mat4& transform);

    virtual void onEnter();

    virtual void onExit();

    virtual void addChild(Node* child);

    virtual void removeChild(Node* child);

    virtual size_t getChildCount() const;

    virtual void setParent(Node* parent);

    virtual Node* getParent() const;

    virtual void sortAllChildren();

    template <typename T>
    inline static void sortNodes(std::vector<T*>& nodes)
    {
        static_assert(std::is_base_of<Node, T>::value,
                      "Node::sortNodes: Only accept derived of Node!");
        std::sort(std::begin(nodes), std::end(nodes),
                  [](T* n1, T* n2) { return (n1->m_localZOrder < n2->m_localZOrder); });
    }

    std::string getName() const;
    void setName(const std::string& name);

    int32_t getLocalZOrder() const { return m_localZOrder; }
    void setLocalZOrder(int32_t localZOrder);

    float getGlobalZOrder() const { return m_globalZOrder; }
    void setGlobalZOrder(float globalZOrder);

    virtual void visit(Renderer* renderer, const math::mat4& transform, uint32_t parentFlags);

protected:
    Node* m_parent = nullptr;       //!< Parent node
    std::vector<Node*> m_children;  //!< Child nodes
    std::string m_name;             //!< Node name
    int32_t m_localZOrder = 0;      //!< Local Z order of the node
    float m_globalZOrder = 0.0f;    //!< Global Z order of the node
};

} // namespace ocf
