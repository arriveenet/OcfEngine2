#include <gtest/gtest.h>

#include "renderer/backend/DriverBase.h"

using namespace ocf::backend;

// BufferDescripter basic construction test
TEST(BufferDescripterTest, DefaultConstructor)
{
    BufferDescripter desc;
    EXPECT_EQ(desc.size, 0u);
    EXPECT_EQ(desc.usage, BufferUsage::STATIC);
}

// BufferDescripter parameterized constructor test
TEST(BufferDescripterTest, ParameterizedConstructor)
{
    BufferDescripter desc(1024, BufferUsage::DYNAMIC);
    EXPECT_EQ(desc.size, 1024u);
    EXPECT_EQ(desc.usage, BufferUsage::DYNAMIC);
}

// BufferDescripter move semantics test
TEST(BufferDescripterTest, MoveSemantics)
{
    BufferDescripter desc1(1024, BufferUsage::DYNAMIC);
    BufferDescripter desc2 = std::move(desc1);
    
    EXPECT_EQ(desc2.size, 1024u);
    EXPECT_EQ(desc2.usage, BufferUsage::DYNAMIC);
    
    BufferDescripter desc3;
    desc3 = std::move(desc2);
    EXPECT_EQ(desc3.size, 1024u);
    EXPECT_EQ(desc3.usage, BufferUsage::DYNAMIC);
}

// PixelBufferDescripter default constructor test
TEST(PixelBufferDescripterTest, DefaultConstructor)
{
    PixelBufferDescripter desc;
    EXPECT_EQ(desc.size, 0u);
    EXPECT_EQ(desc.usage, BufferUsage::STATIC);
    EXPECT_EQ(desc.width, 0u);
    EXPECT_EQ(desc.height, 0u);
    EXPECT_EQ(desc.format, TextureFormat::RGBA8);
}

// PixelBufferDescripter parameterized constructor test
TEST(PixelBufferDescripterTest, ParameterizedConstructor)
{
    PixelBufferDescripter desc(4096, BufferUsage::DYNAMIC, 1024, 1024, TextureFormat::RGB8);
    
    EXPECT_EQ(desc.size, 4096u);
    EXPECT_EQ(desc.usage, BufferUsage::DYNAMIC);
    EXPECT_EQ(desc.width, 1024u);
    EXPECT_EQ(desc.height, 1024u);
    EXPECT_EQ(desc.format, TextureFormat::RGB8);
}

// PixelBufferDescripter move semantics test
TEST(PixelBufferDescripterTest, MoveSemantics)
{
    PixelBufferDescripter desc1(4096, BufferUsage::DYNAMIC, 1024, 1024, TextureFormat::RGB8);
    PixelBufferDescripter desc2 = std::move(desc1);
    
    EXPECT_EQ(desc2.size, 4096u);
    EXPECT_EQ(desc2.usage, BufferUsage::DYNAMIC);
    EXPECT_EQ(desc2.width, 1024u);
    EXPECT_EQ(desc2.height, 1024u);
    EXPECT_EQ(desc2.format, TextureFormat::RGB8);
    
    PixelBufferDescripter desc3;
    desc3 = std::move(desc2);
    EXPECT_EQ(desc3.size, 4096u);
    EXPECT_EQ(desc3.usage, BufferUsage::DYNAMIC);
    EXPECT_EQ(desc3.width, 1024u);
    EXPECT_EQ(desc3.height, 1024u);
    EXPECT_EQ(desc3.format, TextureFormat::RGB8);
}

// PixelBufferDescripter inheritance test
TEST(PixelBufferDescripterTest, InheritanceFromBufferDescripter)
{
    PixelBufferDescripter pixelDesc(2048, BufferUsage::STATIC, 512, 512, TextureFormat::RG8);
    
    // Should be able to use as BufferDescripter
    BufferDescripter& baseDesc = pixelDesc;
    
    EXPECT_EQ(baseDesc.size, 2048u);
    EXPECT_EQ(baseDesc.usage, BufferUsage::STATIC);
}

// Test that copy operations are deleted (should not compile if enabled)
// Uncomment these to verify they don't compile:
/*
TEST(BufferDescripterTest, CopyIsDeleted)
{
    BufferDescripter desc1(1024, BufferUsage::DYNAMIC);
    BufferDescripter desc2 = desc1; // Should not compile
    BufferDescripter desc3;
    desc3 = desc1; // Should not compile
}

TEST(PixelBufferDescripterTest, CopyIsDeleted)
{
    PixelBufferDescripter desc1(1024, BufferUsage::DYNAMIC, 512, 512, TextureFormat::RGBA8);
    PixelBufferDescripter desc2 = desc1; // Should not compile
    PixelBufferDescripter desc3;
    desc3 = desc1; // Should not compile
}
*/