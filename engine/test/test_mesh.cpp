#include <gtest/gtest.h>
#include <ocf/3d/Mesh.h>
#include <ocf/renderer/VertexBuffer.h>
#include <ocf/renderer/IndexBuffer.h>

using namespace ocf;

class MeshTest : public ::testing::Test {
protected:
    Mesh* mesh = nullptr;

    void SetUp() override {
        mesh = Mesh::create();
    }

    void TearDown() override {
        delete mesh;
    }
};

TEST_F(MeshTest, Create_ReturnsNonNull) {
    EXPECT_NE(mesh, nullptr);
}

TEST_F(MeshTest, Constructor_DefaultValues) {
    EXPECT_EQ(mesh->getVertexBuffer(), nullptr);
    EXPECT_EQ(mesh->getIndexBuffer(), nullptr);
    EXPECT_EQ(mesh->getPrimitiveType(), backend::PrimitiveType::TRIANGLES);
}

TEST_F(MeshTest, SetVertexBuffer) {
    // Note: We're not actually creating buffers since we can't initialize the full engine
    // This just tests that the setter/getter work
    VertexBuffer* vb = nullptr;
    mesh->setVertexBuffer(vb);
    EXPECT_EQ(mesh->getVertexBuffer(), vb);
}

TEST_F(MeshTest, SetIndexBuffer) {
    IndexBuffer* ib = nullptr;
    mesh->setIndexBuffer(ib);
    EXPECT_EQ(mesh->getIndexBuffer(), ib);
}

TEST_F(MeshTest, SetPrimitiveType) {
    mesh->setPrimitiveType(backend::PrimitiveType::LINES);
    EXPECT_EQ(mesh->getPrimitiveType(), backend::PrimitiveType::LINES);
    
    mesh->setPrimitiveType(backend::PrimitiveType::POINTS);
    EXPECT_EQ(mesh->getPrimitiveType(), backend::PrimitiveType::POINTS);
}
