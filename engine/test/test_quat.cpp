#include <gtest/gtest.h>
#include <cmath>

#include <ocf/math/quat.h>
#include <ocf/math/vec3.h>

using namespace ocf::math;

// quatのデフォルトコンストラクタのテスト
TEST(QuatTest, DefaultConstructor)
{
    quat_t q;
    // デフォルトコンストラクタで初期化された値をテスト
}

// quatのスカラーコンストラクタのテスト（単位クォータニオン）
TEST(QuatTest, ScalarConstructorIdentity)
{
    quat_t q(1.0f);
    
    // スカラー部が1、ベクトル部が0であることを確認
    EXPECT_FLOAT_EQ(q.x, 0.0f);
    EXPECT_FLOAT_EQ(q.y, 0.0f);
    EXPECT_FLOAT_EQ(q.z, 0.0f);
    EXPECT_FLOAT_EQ(q.w, 1.0f);
}

// quatの成分指定コンストラクタのテスト
TEST(QuatTest, ComponentConstructor)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    EXPECT_FLOAT_EQ(q.x, 1.0f);
    EXPECT_FLOAT_EQ(q.y, 2.0f);
    EXPECT_FLOAT_EQ(q.z, 3.0f);
    EXPECT_FLOAT_EQ(q.w, 4.0f);
}

// quatの軸角度コンストラクタのテスト
TEST(QuatTest, AxisAngleConstructor)
{
    vec3 axis(0.0f, 0.0f, 1.0f);  // Z軸
    float angle = M_PI / 2.0f;    // 90度
    quat_t q(axis, angle);
    
    // Z軸周りの90度回転のクォータニオン
    EXPECT_FLOAT_EQ(q.x, 0.0f);
    EXPECT_FLOAT_EQ(q.y, 0.0f);
    EXPECT_NEAR(q.z, std::sin(M_PI / 4.0f), 1e-6f);  // sin(45°)
    EXPECT_NEAR(q.w, std::cos(M_PI / 4.0f), 1e-6f);  // cos(45°)
}

// 配列アクセス演算子のテスト
TEST(QuatTest, ArrayAccessOperator)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    EXPECT_FLOAT_EQ(q[0], 1.0f);
    EXPECT_FLOAT_EQ(q[1], 2.0f);
    EXPECT_FLOAT_EQ(q[2], 3.0f);
    EXPECT_FLOAT_EQ(q[3], 4.0f);
}

// クォータニオン加算のテスト
TEST(QuatTest, QuaternionAddition)
{
    quat_t q1(1.0f, 2.0f, 3.0f, 4.0f);
    quat_t q2(1.0f, 1.0f, 1.0f, 1.0f);
    
    quat_t result = q1 + q2;
    
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
    EXPECT_FLOAT_EQ(result.z, 4.0f);
    EXPECT_FLOAT_EQ(result.w, 5.0f);
}

// クォータニオン減算のテスト
TEST(QuatTest, QuaternionSubtraction)
{
    quat_t q1(3.0f, 4.0f, 5.0f, 6.0f);
    quat_t q2(1.0f, 1.0f, 1.0f, 1.0f);
    
    quat_t result = q1 - q2;
    
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
    EXPECT_FLOAT_EQ(result.z, 4.0f);
    EXPECT_FLOAT_EQ(result.w, 5.0f);
}

// クォータニオンとスカラーの乗算のテスト
TEST(QuatTest, QuaternionScalarMultiplication)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    quat_t result = q * 2.0f;
    
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 4.0f);
    EXPECT_FLOAT_EQ(result.z, 6.0f);
    EXPECT_FLOAT_EQ(result.w, 8.0f);
}

// クォータニオン乗算のテスト（単位クォータニオン）
TEST(QuatTest, QuaternionMultiplicationIdentity)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    quat_t identity(1.0f);  // 単位クォータニオン
    
    quat_t result = q * identity;
    
    EXPECT_FLOAT_EQ(result.x, 1.0f);
    EXPECT_FLOAT_EQ(result.y, 2.0f);
    EXPECT_FLOAT_EQ(result.z, 3.0f);
    EXPECT_FLOAT_EQ(result.w, 4.0f);
}

// 等価比較演算子のテスト
TEST(QuatTest, EqualityOperator)
{
    quat_t q1(1.0f, 2.0f, 3.0f, 4.0f);
    quat_t q2(1.0f, 2.0f, 3.0f, 4.0f);
    
    EXPECT_TRUE(q1 == q2);
}

// 非等価比較演算子のテスト
TEST(QuatTest, InequalityOperator)
{
    quat_t q1(1.0f, 2.0f, 3.0f, 4.0f);
    quat_t q2(1.0f, 2.0f, 3.0f, 5.0f);
    
    EXPECT_TRUE(q1 != q2);
}

// 長さ（ノルム）のテスト
TEST(QuatTest, Length)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    float len = length(q);
    float expected = std::sqrt(1.0f + 4.0f + 9.0f + 16.0f);  // sqrt(30)
    
    EXPECT_NEAR(len, expected, 1e-6f);
}

// 正規化のテスト
TEST(QuatTest, Normalize)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    quat_t normalized = normalize(q);
    float len = length(normalized);
    
    EXPECT_NEAR(len, 1.0f, 1e-6f);
}

// 内積のテスト
TEST(QuatTest, DotProduct)
{
    quat_t q1(1.0f, 2.0f, 3.0f, 4.0f);
    quat_t q2(2.0f, 3.0f, 4.0f, 5.0f);
    
    float dotProduct = dot(q1, q2);
    float expected = 1.0f * 2.0f + 2.0f * 3.0f + 3.0f * 4.0f + 4.0f * 5.0f;  // 40
    
    EXPECT_FLOAT_EQ(dotProduct, expected);
}

// 共役のテスト
TEST(QuatTest, Conjugate)
{
    quat_t q(1.0f, 2.0f, 3.0f, 4.0f);
    
    quat_t conj = conjugate(q);
    
    EXPECT_FLOAT_EQ(conj.x, -1.0f);
    EXPECT_FLOAT_EQ(conj.y, -2.0f);
    EXPECT_FLOAT_EQ(conj.z, -3.0f);
    EXPECT_FLOAT_EQ(conj.w, 4.0f);
}

// 逆クォータニオンのテスト
TEST(QuatTest, Inverse)
{
    quat_t q(0.0f, 0.0f, 0.0f, 1.0f);  // 単位クォータニオン
    
    quat_t inv = inverse(q);
    quat_t identity = q * inv;
    
    // 逆クォータニオンとの乗算が単位クォータニオンになることを確認
    EXPECT_NEAR(identity.x, 0.0f, 1e-6f);
    EXPECT_NEAR(identity.y, 0.0f, 1e-6f);
    EXPECT_NEAR(identity.z, 0.0f, 1e-6f);
    EXPECT_NEAR(identity.w, 1.0f, 1e-6f);
}

// ベクトル回転のテスト
TEST(QuatTest, RotateVector)
{
    // Z軸周りの90度回転
    vec3 axis(0.0f, 0.0f, 1.0f);
    quat_t q(axis, M_PI / 2.0f);
    
    vec3 v(1.0f, 0.0f, 0.0f);  // X軸ベクトル
    vec3 rotated = rotate(q, v);
    
    // 90度回転後はY軸方向になるはず
    EXPECT_NEAR(rotated.x, 0.0f, 1e-6f);
    EXPECT_NEAR(rotated.y, 1.0f, 1e-6f);
    EXPECT_NEAR(rotated.z, 0.0f, 1e-6f);
}

// angleAxis関数のテスト
TEST(QuatTest, AngleAxis)
{
    vec3 axis(0.0f, 1.0f, 0.0f);  // Y軸
    float angle = M_PI / 3.0f;    // 60度
    
    quat_t q = angleAxis(angle, axis);
    
    EXPECT_NEAR(q.x, 0.0f, 1e-6f);
    EXPECT_NEAR(q.y, std::sin(M_PI / 6.0f), 1e-6f);  // sin(30°)
    EXPECT_NEAR(q.z, 0.0f, 1e-6f);
    EXPECT_NEAR(q.w, std::cos(M_PI / 6.0f), 1e-6f);  // cos(30°)
}

// slerp（球面線形補間）のテスト
TEST(QuatTest, Slerp)
{
    quat_t q1(0.0f, 0.0f, 0.0f, 1.0f);  // 単位クォータニオン
    quat_t q2(0.0f, 0.0f, 0.707107f, 0.707107f);  // Z軸周り90度回転
    
    quat_t result = slerp(q1, q2, 0.5f);  // 中間点
    
    // 45度回転になっているはず
    EXPECT_NEAR(result.x, 0.0f, 1e-5f);
    EXPECT_NEAR(result.y, 0.0f, 1e-5f);
    EXPECT_NEAR(result.z, 0.382683f, 1e-5f);  // sin(22.5°)
    EXPECT_NEAR(result.w, 0.92388f, 1e-5f);   // cos(22.5°)
}

// 型エイリアスのテスト
TEST(QuatTest, TypeAliases)
{
    quat_t q_float(1.0f);
    dquat q_double(1.0);
    
    EXPECT_FLOAT_EQ(q_float.w, 1.0f);
    EXPECT_DOUBLE_EQ(q_double.w, 1.0);
}