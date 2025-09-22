/* SPDX - License - Identifier : MIT */
#pragma once
#include "ocf/base/Object.h"
#include "ocf/math/mat4.h"
#include <string>
#include <vector>

namespace ocf {

class Renderer;

class Node : public Object {
public:
    Node();
    virtual ~Node();

    virtual bool init();

    virtual void update(float deltaTime);

    virtual void draw(Renderer* renderer, const math::mat4& transform);

    virtual void onEnter();

    virtual void onExit();

    virtual void addChild(Node* child);

    virtual void removeChild(Node* child);

    virtual size_t getChildCount() const;

    virtual void setParent(Node* parent);

    virtual Node* getParent() const;

    std::string getName() const;
    void setName(const std::string& name);

    virtual void visit(Renderer* renderer, const math::mat4& transform, uint32_t parentFlags);

protected:
    Node* m_parent = nullptr;       //!< Parent node
    std::vector<Node*> m_children;  //!< Child nodes
    std::string m_name;             //!< Node name
};

} // namespace ocf
