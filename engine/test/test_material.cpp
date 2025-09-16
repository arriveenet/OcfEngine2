#include <gtest/gtest.h>
#include "ocf/renderer/Material.h"
#include "ocf/math/vec2.h"
#include "ocf/math/vec3.h"
#include "ocf/math/vec4.h"

using namespace ocf;

class MaterialTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Note: Material creation requires an initialized engine/driver
        // For these tests, we'll test the basic Material functionality
        // without actually creating materials through the driver
    }

    void TearDown() override {
    }
};

TEST_F(MaterialTest, BlendModeSettersAndGetters)
{
    Material material;
    
    // Test default blend mode
    EXPECT_EQ(Material::BlendMode::OPAQUE, material.getBlendMode());
    
    // Test setting and getting different blend modes
    material.setBlendMode(Material::BlendMode::TRANSPARENT);
    EXPECT_EQ(Material::BlendMode::TRANSPARENT, material.getBlendMode());
    
    material.setBlendMode(Material::BlendMode::ADD);
    EXPECT_EQ(Material::BlendMode::ADD, material.getBlendMode());
    
    material.setBlendMode(Material::BlendMode::MULTIPLY);
    EXPECT_EQ(Material::BlendMode::MULTIPLY, material.getBlendMode());
}

TEST_F(MaterialTest, CullModeSettersAndGetters)
{
    Material material;
    
    // Test default cull mode
    EXPECT_EQ(Material::CullMode::BACK, material.getCullMode());
    
    // Test setting and getting different cull modes
    material.setCullMode(Material::CullMode::NONE);
    EXPECT_EQ(Material::CullMode::NONE, material.getCullMode());
    
    material.setCullMode(Material::CullMode::FRONT);
    EXPECT_EQ(Material::CullMode::FRONT, material.getCullMode());
    
    material.setCullMode(Material::CullMode::FRONT_AND_BACK);
    EXPECT_EQ(Material::CullMode::FRONT_AND_BACK, material.getCullMode());
}

TEST_F(MaterialTest, DepthTestSettersAndGetters)
{
    Material material;
    
    // Test default depth test setting
    EXPECT_TRUE(material.getDepthTest());
    
    // Test setting and getting depth test
    material.setDepthTest(false);
    EXPECT_FALSE(material.getDepthTest());
    
    material.setDepthTest(true);
    EXPECT_TRUE(material.getDepthTest());
}

TEST_F(MaterialTest, DepthWriteSettersAndGetters)
{
    Material material;
    
    // Test default depth write setting
    EXPECT_TRUE(material.getDepthWrite());
    
    // Test setting and getting depth write
    material.setDepthWrite(false);
    EXPECT_FALSE(material.getDepthWrite());
    
    material.setDepthWrite(true);
    EXPECT_TRUE(material.getDepthWrite());
}

TEST_F(MaterialTest, FloatParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting float parameters
    material.setParameter("testFloat", 3.14f);
    
    float value;
    EXPECT_TRUE(material.getParameter("testFloat", value));
    EXPECT_FLOAT_EQ(3.14f, value);
    
    // Test getting non-existent parameter
    EXPECT_FALSE(material.getParameter("nonExistent", value));
}

TEST_F(MaterialTest, Vec2ParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting vec2 parameters
    math::vec2 testVec(1.0f, 2.0f);
    material.setParameter("testVec2", testVec);
    
    math::vec2 value;
    EXPECT_TRUE(material.getParameter("testVec2", value));
    EXPECT_FLOAT_EQ(1.0f, value.x);
    EXPECT_FLOAT_EQ(2.0f, value.y);
    
    // Test getting wrong type
    float floatValue;
    EXPECT_FALSE(material.getParameter("testVec2", floatValue));
}

TEST_F(MaterialTest, Vec3ParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting vec3 parameters
    math::vec3 testVec(1.0f, 2.0f, 3.0f);
    material.setParameter("testVec3", testVec);
    
    math::vec3 value;
    EXPECT_TRUE(material.getParameter("testVec3", value));
    EXPECT_FLOAT_EQ(1.0f, value.x);
    EXPECT_FLOAT_EQ(2.0f, value.y);
    EXPECT_FLOAT_EQ(3.0f, value.z);
}

TEST_F(MaterialTest, Vec4ParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting vec4 parameters
    math::vec4 testVec(1.0f, 2.0f, 3.0f, 4.0f);
    material.setParameter("testVec4", testVec);
    
    math::vec4 value;
    EXPECT_TRUE(material.getParameter("testVec4", value));
    EXPECT_FLOAT_EQ(1.0f, value.x);
    EXPECT_FLOAT_EQ(2.0f, value.y);
    EXPECT_FLOAT_EQ(3.0f, value.z);
    EXPECT_FLOAT_EQ(4.0f, value.w);
}

TEST_F(MaterialTest, IntParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting int parameters
    material.setParameter("testInt", 42);
    
    int value;
    EXPECT_TRUE(material.getParameter("testInt", value));
    EXPECT_EQ(42, value);
}

TEST_F(MaterialTest, BoolParameterSettersAndGetters)
{
    Material material;
    
    // Test setting and getting bool parameters
    material.setParameter("testBool", true);
    
    bool value;
    EXPECT_TRUE(material.getParameter("testBool", value));
    EXPECT_TRUE(value);
    
    material.setParameter("testBool", false);
    EXPECT_TRUE(material.getParameter("testBool", value));
    EXPECT_FALSE(value);
}