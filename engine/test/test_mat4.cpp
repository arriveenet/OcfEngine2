#include <gtest/gtest.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <ocf/math/mat4.h>
#include <ocf/math/vec4.h>

using namespace ocf::math;

// mat4のデフォルトコンストラクタのテスト
TEST(Mat4Test, DefaultConstructor)
{
    mat4 m;
    // デフォルトコンストラクタで初期化された値をテスト
}

// mat4のスカラーコンストラクタのテスト（単位行列）
TEST(Mat4Test, ScalarConstructorIdentity)
{
    mat4 m(1.0f);
    
    // 対角成分が1であることを確認
    EXPECT_FLOAT_EQ(m[0].x, 1.0f);
    EXPECT_FLOAT_EQ(m[1].y, 1.0f);
    EXPECT_FLOAT_EQ(m[2].z, 1.0f);
    EXPECT_FLOAT_EQ(m[3].w, 1.0f);
    
    // 非対角成分が0であることを確認
    EXPECT_FLOAT_EQ(m[0].y, 0.0f);
    EXPECT_FLOAT_EQ(m[0].z, 0.0f);
    EXPECT_FLOAT_EQ(m[0].w, 0.0f);
    EXPECT_FLOAT_EQ(m[1].x, 0.0f);
    EXPECT_FLOAT_EQ(m[1].z, 0.0f);
    EXPECT_FLOAT_EQ(m[1].w, 0.0f);
    EXPECT_FLOAT_EQ(m[2].x, 0.0f);
    EXPECT_FLOAT_EQ(m[2].y, 0.0f);
    EXPECT_FLOAT_EQ(m[2].w, 0.0f);
    EXPECT_FLOAT_EQ(m[3].x, 0.0f);
    EXPECT_FLOAT_EQ(m[3].y, 0.0f);
    EXPECT_FLOAT_EQ(m[3].z, 0.0f);
}

// mat4のスカラーコンストラクタのテスト（任意の値）
TEST(Mat4Test, ScalarConstructorArbitraryValue)
{
    mat4 m(2.5f);
    
    // 対角成分が2.5であることを確認
    EXPECT_FLOAT_EQ(m[0].x, 2.5f);
    EXPECT_FLOAT_EQ(m[1].y, 2.5f);
    EXPECT_FLOAT_EQ(m[2].z, 2.5f);
    EXPECT_FLOAT_EQ(m[3].w, 2.5f);
    
    // 非対角成分が0であることを確認
    EXPECT_FLOAT_EQ(m[0].y, 0.0f);
    EXPECT_FLOAT_EQ(m[1].x, 0.0f);
}

// mat4の成分指定コンストラクタのテスト
TEST(Mat4Test, ComponentConstructor)
{
    mat4 m(1.0f, 2.0f, 3.0f, 4.0f,
           5.0f, 6.0f, 7.0f, 8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
           13.0f, 14.0f, 15.0f, 16.0f);
    
    EXPECT_FLOAT_EQ(m[0].x, 1.0f);
    EXPECT_FLOAT_EQ(m[0].y, 2.0f);
    EXPECT_FLOAT_EQ(m[0].z, 3.0f);
    EXPECT_FLOAT_EQ(m[0].w, 4.0f);
    
    EXPECT_FLOAT_EQ(m[1].x, 5.0f);
    EXPECT_FLOAT_EQ(m[1].y, 6.0f);
    EXPECT_FLOAT_EQ(m[1].z, 7.0f);
    EXPECT_FLOAT_EQ(m[1].w, 8.0f);
    
    EXPECT_FLOAT_EQ(m[2].x, 9.0f);
    EXPECT_FLOAT_EQ(m[2].y, 10.0f);
    EXPECT_FLOAT_EQ(m[2].z, 11.0f);
    EXPECT_FLOAT_EQ(m[2].w, 12.0f);
    
    EXPECT_FLOAT_EQ(m[3].x, 13.0f);
    EXPECT_FLOAT_EQ(m[3].y, 14.0f);
    EXPECT_FLOAT_EQ(m[3].z, 15.0f);
    EXPECT_FLOAT_EQ(m[3].w, 16.0f);
}

// mat4のカラムベクターコンストラクタのテスト
TEST(Mat4Test, ColumnConstructor)
{
    vec4 c0(1.0f, 2.0f, 3.0f, 4.0f);
    vec4 c1(5.0f, 6.0f, 7.0f, 8.0f);
    vec4 c2(9.0f, 10.0f, 11.0f, 12.0f);
    vec4 c3(13.0f, 14.0f, 15.0f, 16.0f);
    
    mat4 m(c0, c1, c2, c3);
    
    EXPECT_FLOAT_EQ(m[0].x, 1.0f);
    EXPECT_FLOAT_EQ(m[0].y, 2.0f);
    EXPECT_FLOAT_EQ(m[0].z, 3.0f);
    EXPECT_FLOAT_EQ(m[0].w, 4.0f);
    
    EXPECT_FLOAT_EQ(m[1].x, 5.0f);
    EXPECT_FLOAT_EQ(m[1].y, 6.0f);
    EXPECT_FLOAT_EQ(m[1].z, 7.0f);
    EXPECT_FLOAT_EQ(m[1].w, 8.0f);
}

// 配列アクセス演算子のテスト
TEST(Mat4Test, ArrayAccessOperator)
{
    mat4 m(1.0f);
    
    // 読み込みテスト
    EXPECT_FLOAT_EQ(m[0].x, 1.0f);
    EXPECT_FLOAT_EQ(m[1].y, 1.0f);
    
    // 書き込みテスト
    m[0].x = 5.0f;
    m[1].y = 6.0f;
    
    EXPECT_FLOAT_EQ(m[0].x, 5.0f);
    EXPECT_FLOAT_EQ(m[1].y, 6.0f);
}

// 行列加算のテスト
TEST(Mat4Test, MatrixAddition)
{
    mat4 m1(1.0f, 2.0f, 3.0f, 4.0f,
            5.0f, 6.0f, 7.0f, 8.0f,
            9.0f, 10.0f, 11.0f, 12.0f,
            13.0f, 14.0f, 15.0f, 16.0f);
    
    mat4 m2(1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f);
    
    mat4 result = m1 + m2;
    
    EXPECT_FLOAT_EQ(result[0].x, 2.0f);
    EXPECT_FLOAT_EQ(result[0].y, 3.0f);
    EXPECT_FLOAT_EQ(result[1].x, 6.0f);
    EXPECT_FLOAT_EQ(result[3].w, 17.0f);
}

// 行列減算のテスト
TEST(Mat4Test, MatrixSubtraction)
{
    mat4 m1(2.0f, 3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f, 9.0f,
            10.0f, 11.0f, 12.0f, 13.0f,
            14.0f, 15.0f, 16.0f, 17.0f);
    
    mat4 m2(1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f);
    
    mat4 result = m1 - m2;
    
    EXPECT_FLOAT_EQ(result[0].x, 1.0f);
    EXPECT_FLOAT_EQ(result[0].y, 2.0f);
    EXPECT_FLOAT_EQ(result[1].x, 5.0f);
    EXPECT_FLOAT_EQ(result[3].w, 16.0f);
}

// 行列とスカラーの乗算のテスト
TEST(Mat4Test, MatrixScalarMultiplication)
{
    mat4 m(1.0f, 2.0f, 3.0f, 4.0f,
           5.0f, 6.0f, 7.0f, 8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
           13.0f, 14.0f, 15.0f, 16.0f);
    
    mat4 result = m * 2.0f;
    
    EXPECT_FLOAT_EQ(result[0].x, 2.0f);
    EXPECT_FLOAT_EQ(result[0].y, 4.0f);
    EXPECT_FLOAT_EQ(result[1].x, 10.0f);
    EXPECT_FLOAT_EQ(result[3].w, 32.0f);
}

// 行列乗算のテスト（単位行列）
TEST(Mat4Test, MatrixMultiplicationIdentity)
{
    mat4 identity(1.0f);
    mat4 m(1.0f, 2.0f, 3.0f, 4.0f,
           5.0f, 6.0f, 7.0f, 8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
           13.0f, 14.0f, 15.0f, 16.0f);
    
    mat4 result = identity * m;
    
    // 単位行列との乗算は元の行列と同じであるべき
    EXPECT_FLOAT_EQ(result[0].x, 1.0f);
    EXPECT_FLOAT_EQ(result[0].y, 2.0f);
    EXPECT_FLOAT_EQ(result[1].x, 5.0f);
    EXPECT_FLOAT_EQ(result[3].w, 16.0f);
}

// 行列とベクトルの乗算のテスト
TEST(Mat4Test, MatrixVectorMultiplication)
{
    mat4 identity(1.0f);
    vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
    
    vec4 result = identity * v;
    
    // 単位行列との乗算は元のベクトルと同じであるべき
    EXPECT_FLOAT_EQ(result.x, 1.0f);
    EXPECT_FLOAT_EQ(result.y, 2.0f);
    EXPECT_FLOAT_EQ(result.z, 3.0f);
    EXPECT_FLOAT_EQ(result.w, 4.0f);
}

// 等価比較演算子のテスト
TEST(Mat4Test, EqualityOperator)
{
    mat4 m1(1.0f);
    mat4 m2(1.0f);
    mat4 m3(2.0f);
    
    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
}

// 非等価比較演算子のテスト
TEST(Mat4Test, InequalityOperator)
{
    mat4 m1(1.0f);
    mat4 m2(1.0f);
    mat4 m3(2.0f);
    
    EXPECT_FALSE(m1 != m2);
    EXPECT_TRUE(m1 != m3);
}

// 転置行列のテスト
TEST(Mat4Test, Transpose)
{
    mat4 m(1.0f, 2.0f, 3.0f, 4.0f,
           5.0f, 6.0f, 7.0f, 8.0f,
           9.0f, 10.0f, 11.0f, 12.0f,
           13.0f, 14.0f, 15.0f, 16.0f);
    
    mat4 result = transpose(m);
    
    // 転置後の値をテスト
    EXPECT_FLOAT_EQ(result[0].x, 1.0f);  // m[0][0]
    EXPECT_FLOAT_EQ(result[1].x, 2.0f);  // m[0][1]
    EXPECT_FLOAT_EQ(result[2].x, 3.0f);  // m[0][2]
    EXPECT_FLOAT_EQ(result[3].x, 4.0f);  // m[0][3]
    
    EXPECT_FLOAT_EQ(result[0].y, 5.0f);  // m[1][0]
    EXPECT_FLOAT_EQ(result[1].y, 6.0f);  // m[1][1]
}

// 行列式のテスト（単位行列）
TEST(Mat4Test, DeterminantIdentity)
{
    mat4 identity(1.0f);
    float det = determinant(identity);
    
    EXPECT_FLOAT_EQ(det, 1.0f);
}

// 逆行列のテスト（単位行列）
TEST(Mat4Test, InverseIdentity)
{
    mat4 identity(1.0f);
    mat4 result = inverse(identity);
    
    // 単位行列の逆行列は単位行列であるべき
    EXPECT_FLOAT_EQ(result[0].x, 1.0f);
    EXPECT_FLOAT_EQ(result[1].y, 1.0f);
    EXPECT_FLOAT_EQ(result[2].z, 1.0f);
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
    
    // 非対角成分は0であるべき
    EXPECT_FLOAT_EQ(result[0].y, 0.0f);
    EXPECT_FLOAT_EQ(result[1].x, 0.0f);
}

// 型エイリアスのテスト
TEST(Mat4Test, TypeAliases)
{
    // mat4はmat<4,4,float>と同じであることを確認
    mat4 m1(1.0f);
    mat<4, 4, float> m2(1.0f);
    
    EXPECT_TRUE(m1 == m2);
}

// doubleの行列のテスト
TEST(Mat4Test, DoublePrecisionMatrix)
{
    dmat4 m(1.0);
    
    EXPECT_DOUBLE_EQ(m[0].x, 1.0);
    EXPECT_DOUBLE_EQ(m[1].y, 1.0);
    EXPECT_DOUBLE_EQ(m[2].z, 1.0);
    EXPECT_DOUBLE_EQ(m[3].w, 1.0);
}

// 平行移動行列のテスト
TEST(Mat4Test, TranslateMatrix)
{
    mat4 identity(1.0f);
    vec3 translation(2.0f, 3.0f, 4.0f);
    
    mat4 result = translate(identity, translation);
    
    // 平行移動行列の検証
    EXPECT_FLOAT_EQ(result[0].x, 1.0f);
    EXPECT_FLOAT_EQ(result[1].y, 1.0f);
    EXPECT_FLOAT_EQ(result[2].z, 1.0f);
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
    
    // 平行移動成分の検証
    EXPECT_FLOAT_EQ(result[3].x, 2.0f);
    EXPECT_FLOAT_EQ(result[3].y, 3.0f);
    EXPECT_FLOAT_EQ(result[3].z, 4.0f);
}

// 回転行列のテスト（Z軸回転）
TEST(Mat4Test, RotateMatrix)
{
    mat4 identity(1.0f);
    float angle = static_cast<float>(M_PI / 2.0f); // 90度
    vec3 axis(0.0f, 0.0f, 1.0f); // Z軸
    
    mat4 result = rotate(identity, angle, axis);
    
    // Z軸90度回転の検証（近似値での比較）
    EXPECT_NEAR(result[0].x, 0.0f, 1e-6f);
    EXPECT_NEAR(result[0].y, 1.0f, 1e-6f);
    EXPECT_NEAR(result[1].x, -1.0f, 1e-6f);
    EXPECT_NEAR(result[1].y, 0.0f, 1e-6f);
    EXPECT_FLOAT_EQ(result[2].z, 1.0f);
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
}

// スケール行列のテスト
TEST(Mat4Test, ScaleMatrix)
{
    mat4 identity(1.0f);
    vec3 scale_factor(2.0f, 3.0f, 4.0f);
    
    mat4 result = scale(identity, scale_factor);
    
    // スケール行列の検証
    EXPECT_FLOAT_EQ(result[0].x, 2.0f);
    EXPECT_FLOAT_EQ(result[1].y, 3.0f);
    EXPECT_FLOAT_EQ(result[2].z, 4.0f);
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
    
    // 非対角成分は0であるべき
    EXPECT_FLOAT_EQ(result[0].y, 0.0f);
    EXPECT_FLOAT_EQ(result[1].x, 0.0f);
}

// 透視投影行列のテスト
TEST(Mat4Test, PerspectiveMatrix)
{
    float fovy = static_cast<float>(M_PI / 4.0f); // 45度
    float aspect = 16.0f / 9.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    
    mat4 result = perspective(fovy, aspect, zNear, zFar);
    
    // 透視投影行列の基本的な検証
    EXPECT_GT(result[0].x, 0.0f); // X方向のスケーリング
    EXPECT_GT(result[1].y, 0.0f); // Y方向のスケーリング
    EXPECT_LT(result[2].z, 0.0f); // Z方向の変換
    EXPECT_FLOAT_EQ(result[2].w, -1.0f); // 透視分割用
    EXPECT_LT(result[3].z, 0.0f); // 深度オフセット
}

// 正射影行列のテスト
TEST(Mat4Test, OrthoMatrix)
{
    float left = -5.0f;
    float right = 5.0f;
    float bottom = -5.0f;
    float top = 5.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    
    mat4 result = ortho(left, right, bottom, top, zNear, zFar);
    
    // 正射影行列の基本的な検証
    EXPECT_FLOAT_EQ(result[0].x, 0.2f); // 2/(right-left) = 2/10 = 0.2
    EXPECT_FLOAT_EQ(result[1].y, 0.2f); // 2/(top-bottom) = 2/10 = 0.2
    EXPECT_LT(result[2].z, 0.0f); // -2/(zFar-zNear)
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
}

// ビュー行列のテスト
TEST(Mat4Test, LookAtMatrix)
{
    vec3 eye(0.0f, 0.0f, 5.0f);
    vec3 center(0.0f, 0.0f, 0.0f);
    vec3 up(0.0f, 1.0f, 0.0f);
    
    mat4 result = lookAt(eye, center, up);
    
    // ビュー行列の基本的な検証
    EXPECT_FLOAT_EQ(result[0].x, 1.0f); // 右ベクトルX成分
    EXPECT_FLOAT_EQ(result[1].y, 1.0f); // 上ベクトルY成分
    EXPECT_FLOAT_EQ(result[2].z, 1.0f); // 前ベクトルZ成分（反転）
    EXPECT_FLOAT_EQ(result[3].z, -5.0f); // 平行移動成分
    EXPECT_FLOAT_EQ(result[3].w, 1.0f);
}

// 複合変換のテスト
TEST(Mat4Test, CompositeTransformation)
{
    mat4 identity(1.0f);
    
    // 平行移動 → スケールの複合変換
    vec3 translation(1.0f, 2.0f, 3.0f);
    mat4 translated = translate(identity, translation);
    
    vec3 scale_factor(2.0f, 2.0f, 2.0f);
    mat4 final_matrix = scale(translated, scale_factor);
    
    // スケールが適用されていることを確認
    EXPECT_FLOAT_EQ(final_matrix[0].x, 2.0f);
    EXPECT_FLOAT_EQ(final_matrix[1].y, 2.0f);
    EXPECT_FLOAT_EQ(final_matrix[2].z, 2.0f);
    
    // 平行移動は後から適用されたスケールの影響を受けない
    EXPECT_FLOAT_EQ(final_matrix[3].x, 1.0f);
    EXPECT_FLOAT_EQ(final_matrix[3].y, 2.0f);
    EXPECT_FLOAT_EQ(final_matrix[3].z, 3.0f);
}