#include <cmath>
#include <gtest/gtest.h>
#include <ocf/math/noise.h>

using namespace ocf::math;

// Test 2D Perlin noise
TEST(NoiseTest, Perlin2DBasic)
{
    vec<2, float> v1(0.5f, 0.5f);
    float result = perlin(v1);

    // Perlin noise should return a value between -1 and 1
    EXPECT_GE(result, -1.0f);
    EXPECT_LE(result, 1.0f);
}

TEST(NoiseTest, Perlin2DDeterministic)
{
    vec<2, float> v1(1.5f, 2.3f);
    float result1 = perlin(v1);
    float result2 = perlin(v1);

    // Same input should give same output
    EXPECT_FLOAT_EQ(result1, result2);
}

TEST(NoiseTest, Perlin2DContinuous)
{
    vec<2, float> v1(1.0f, 1.0f);
    vec<2, float> v2(1.01f, 1.01f);

    float result1 = perlin(v1);
    float result2 = perlin(v2);

    // Close inputs should give close outputs
    EXPECT_NEAR(result1, result2, 0.1f);
}

TEST(NoiseTest, Perlin2DWithDouble)
{
    vec<2, double> v1(0.5, 0.5);
    double result = perlin(v1);

    EXPECT_GE(result, -1.0);
    EXPECT_LE(result, 1.0);
}

// Test 3D Perlin noise
TEST(NoiseTest, Perlin3DBasic)
{
    vec<3, float> v1(0.5f, 0.5f, 0.5f);
    float result = perlin(v1);

    // Perlin noise should return a value between -1 and 1
    EXPECT_GE(result, -1.0f);
    EXPECT_LE(result, 1.0f);
}

TEST(NoiseTest, Perlin3DDeterministic)
{
    vec<3, float> v1(1.5f, 2.3f, 3.7f);
    float result1 = perlin(v1);
    float result2 = perlin(v1);

    // Same input should give same output
    EXPECT_FLOAT_EQ(result1, result2);
}

TEST(NoiseTest, Perlin3DContinuous)
{
    vec<3, float> v1(1.0f, 1.0f, 1.0f);
    vec<3, float> v2(1.01f, 1.01f, 1.01f);

    float result1 = perlin(v1);
    float result2 = perlin(v2);

    // Close inputs should give close outputs
    EXPECT_NEAR(result1, result2, 0.1f);
}

TEST(NoiseTest, Perlin3DWithDouble)
{
    vec<3, double> v1(0.5, 0.5, 0.5);
    double result = perlin(v1);

    EXPECT_GE(result, -1.0);
    EXPECT_LE(result, 1.0);
}

// Test 4D Perlin noise
TEST(NoiseTest, Perlin4DBasic)
{
    vec<4, float> v1(0.5f, 0.5f, 0.5f, 0.5f);
    float result = perlin(v1);

    // Perlin noise should return a value between -1 and 1
    EXPECT_GE(result, -1.0f);
    EXPECT_LE(result, 1.0f);
}

TEST(NoiseTest, Perlin4DDeterministic)
{
    vec<4, float> v1(1.5f, 2.3f, 3.7f, 4.2f);
    float result1 = perlin(v1);
    float result2 = perlin(v1);

    // Same input should give same output
    EXPECT_FLOAT_EQ(result1, result2);
}

TEST(NoiseTest, Perlin4DContinuous)
{
    vec<4, float> v1(1.0f, 1.0f, 1.0f, 1.0f);
    vec<4, float> v2(1.01f, 1.01f, 1.01f, 1.01f);

    float result1 = perlin(v1);
    float result2 = perlin(v2);

    // Close inputs should give close outputs
    EXPECT_NEAR(result1, result2, 0.1f);
}

TEST(NoiseTest, Perlin4DWithDouble)
{
    vec<4, double> v1(0.5, 0.5, 0.5, 0.5);
    double result = perlin(v1);

    EXPECT_GE(result, -1.0);
    EXPECT_LE(result, 1.0);
}

// Test different coordinates give different results
TEST(NoiseTest, PerlinVariety)
{
    vec<2, float> v1(0.0f, 0.0f);
    vec<2, float> v2(1.0f, 0.0f);
    vec<2, float> v3(0.0f, 1.0f);

    float result1 = perlin(v1);
    float result2 = perlin(v2);
    float result3 = perlin(v3);

    // Different coordinates should (usually) give different results
    // Note: There's a small chance they could be equal, but very unlikely
    EXPECT_TRUE(result1 != result2 || result2 != result3);
}
