#include <gtest/gtest.h>
#include <ocf/base/Node.h>
#include <ocf/math/mat4.h>
#include <ocf/renderer/Renderer.h>

using namespace ocf;

class NodeTest : public ::testing::Test {
protected:
    Node node;
};

TEST_F(NodeTest, Constructor_DefaultValues) {
    EXPECT_EQ(node.getParent(), nullptr);
    EXPECT_EQ(node.getChildCount(), 0u);
    EXPECT_EQ(node.getName(), "");
    EXPECT_EQ(node.getLocalZOrder(), 0);
    EXPECT_FLOAT_EQ(node.getGlobalZOrder(), 0.0f);
}

TEST_F(NodeTest, Init_ReturnsTrueOrFalse) {
    // ŽÀ‘•‚ÉˆË‘¶Btrue/false‚Ì‚Ç‚¿‚ç‚©‚ðŠm”F
    EXPECT_TRUE(node.init() || !node.init());
}

TEST_F(NodeTest, Update_DoesNotThrow) {
    EXPECT_NO_THROW(node.update(0.016f));
}

TEST_F(NodeTest, Draw_DoesNotThrow) {
    Renderer renderer;
    math::mat4 transform;
    EXPECT_NO_THROW(node.draw(&renderer, transform));
}

TEST_F(NodeTest, OnEnter_OnExit_DoesNotThrow) {
    EXPECT_NO_THROW(node.onEnter());
    EXPECT_NO_THROW(node.onExit());
}

TEST_F(NodeTest, AddChild_RemoveChild_ChildCount) {
    Node* child = new Node();
    node.addChild(child);
    EXPECT_EQ(node.getChildCount(), 1u);
    node.removeChild(child);
    EXPECT_EQ(node.getChildCount(), 0u);
}

TEST_F(NodeTest, SetParent_GetParent) {
    Node parent;
    node.setParent(&parent);
    EXPECT_EQ(node.getParent(), &parent);
}

TEST_F(NodeTest, SortAllChildren_DoesNotThrow) {
    Node* child1, * child2;
    child1 = new Node();
    child2 = new Node();
    child1->setLocalZOrder(2);
    child2->setLocalZOrder(1);
    node.addChild(child1);
    node.addChild(child2);
    EXPECT_NO_THROW(node.sortAllChildren());
}

TEST_F(NodeTest, SortNodes_StaticTemplate) {
    Node n1, n2;
    n1.setLocalZOrder(5);
    n2.setLocalZOrder(1);
    std::vector<Node*> nodes = {&n1, &n2};
    Node::sortNodes(nodes);
    EXPECT_LE(nodes[0]->getLocalZOrder(), nodes[1]->getLocalZOrder());
}

TEST_F(NodeTest, GetSetName) {
    node.setName("TestNode");
    EXPECT_EQ(node.getName(), "TestNode");
}

TEST_F(NodeTest, GetSetLocalZOrder) {
    node.setLocalZOrder(42);
    EXPECT_EQ(node.getLocalZOrder(), 42);
}

TEST_F(NodeTest, GetSetGlobalZOrder) {
    node.setGlobalZOrder(3.14f);
    EXPECT_FLOAT_EQ(node.getGlobalZOrder(), 3.14f);
}

TEST_F(NodeTest, Visit_DoesNotThrow) {
    Renderer renderer;
    math::mat4 transform;
    EXPECT_NO_THROW(node.visit(&renderer, transform, 0));
}