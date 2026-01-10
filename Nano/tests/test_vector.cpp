#include <gtest/gtest.h>
#include <Nano/vector.hpp>
#include <cmath>

using namespace robotics::types;

constexpr double kEpsilon = 1e-6;

// 基本的なベクトルの作成テスト
TEST(VectorTest, BasicCreation2D) {
  Vector<double, 2> vec(3.0, 4.0);
  EXPECT_DOUBLE_EQ(vec[0], 3.0);
  EXPECT_DOUBLE_EQ(vec[1], 4.0);
}

TEST(VectorTest, BasicCreation3D) {
  Vector<double, 3> vec(1.0, 2.0, 3.0);
  EXPECT_DOUBLE_EQ(vec[0], 1.0);
  EXPECT_DOUBLE_EQ(vec[1], 2.0);
  EXPECT_DOUBLE_EQ(vec[2], 3.0);
}

TEST(VectorTest, BasicCreation4D) {
  Vector<double, 4> vec(1.0, 2.0, 3.0, 4.0);
  EXPECT_DOUBLE_EQ(vec[0], 1.0);
  EXPECT_DOUBLE_EQ(vec[1], 2.0);
  EXPECT_DOUBLE_EQ(vec[2], 3.0);
  EXPECT_DOUBLE_EQ(vec[3], 4.0);
}

// デフォルトコンストラクタのテスト
TEST(VectorTest, DefaultConstructor) {
  Vector<int, 3> vec;
  EXPECT_EQ(vec[0], 0);
  EXPECT_EQ(vec[1], 0);
  EXPECT_EQ(vec[2], 0);
}

// std::arrayからの構築テスト
TEST(VectorTest, ArrayConstructor) {
  std::array<double, 3> arr = {1.5, 2.5, 3.5};
  Vector<double, 3> vec(arr);
  EXPECT_DOUBLE_EQ(vec[0], 1.5);
  EXPECT_DOUBLE_EQ(vec[1], 2.5);
  EXPECT_DOUBLE_EQ(vec[2], 3.5);
}

// ベクトル加算のテスト
TEST(VectorTest, Addition) {
  Vector<double, 3> v1(1.0, 2.0, 3.0);
  Vector<double, 3> v2(4.0, 5.0, 6.0);
  auto result = v1 + v2;

  EXPECT_DOUBLE_EQ(result[0], 5.0);
  EXPECT_DOUBLE_EQ(result[1], 7.0);
  EXPECT_DOUBLE_EQ(result[2], 9.0);
}

// ベクトル減算のテスト
TEST(VectorTest, Subtraction) {
  Vector<double, 3> v1(5.0, 7.0, 9.0);
  Vector<double, 3> v2(1.0, 2.0, 3.0);
  auto result = v1 - v2;

  EXPECT_DOUBLE_EQ(result[0], 4.0);
  EXPECT_DOUBLE_EQ(result[1], 5.0);
  EXPECT_DOUBLE_EQ(result[2], 6.0);
}

// スカラー乗算のテスト
TEST(VectorTest, ScalarMultiplication) {
  Vector<double, 3> vec(1.0, 2.0, 3.0);
  auto result = vec * 2.0;

  EXPECT_DOUBLE_EQ(result[0], 2.0);
  EXPECT_DOUBLE_EQ(result[1], 4.0);
  EXPECT_DOUBLE_EQ(result[2], 6.0);
}

// スカラー除算のテスト
TEST(VectorTest, ScalarDivision) {
  Vector<double, 3> vec(4.0, 6.0, 8.0);
  auto result = vec / 2.0;

  EXPECT_DOUBLE_EQ(result[0], 2.0);
  EXPECT_DOUBLE_EQ(result[1], 3.0);
  EXPECT_DOUBLE_EQ(result[2], 4.0);
}

// 等価演算子のテスト
TEST(VectorTest, Equality) {
  Vector<int, 3> v1(1, 2, 3);
  Vector<int, 3> v2(1, 2, 3);
  Vector<int, 3> v3(1, 2, 4);

  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);
}

// 非等価演算子のテスト
TEST(VectorTest, Inequality) {
  Vector<int, 3> v1(1, 2, 3);
  Vector<int, 3> v2(1, 2, 4);

  EXPECT_TRUE(v1 != v2);
  EXPECT_FALSE(v1 != v1);
}

// インデックスアクセスのテスト
TEST(VectorTest, IndexAccess) {
  Vector<int, 3> vec(10, 20, 30);

  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);

  vec[1] = 25;
  EXPECT_EQ(vec[1], 25);
}

// const インデックスアクセスのテスト
TEST(VectorTest, ConstIndexAccess) {
  const Vector<int, 3> vec(10, 20, 30);

  EXPECT_EQ(vec[0], 10);
  EXPECT_EQ(vec[1], 20);
  EXPECT_EQ(vec[2], 30);
}

// 大きさ（マグニチュード）のテスト
TEST(VectorTest, Magnitude) {
  Vector<double, 3> vec(3.0, 4.0, 0.0);
  EXPECT_NEAR(vec.Magnitude(), 5.0, kEpsilon);

  Vector<double, 2> vec2(1.0, 1.0);
  EXPECT_NEAR(vec2.Magnitude(), std::sqrt(2.0), kEpsilon);
}

// 正規化のテスト
TEST(VectorTest, Normalized) {
  Vector<double, 3> vec(3.0, 4.0, 0.0);
  auto normalized = vec.Normalized();

  EXPECT_NEAR(normalized[0], 0.6, kEpsilon);
  EXPECT_NEAR(normalized[1], 0.8, kEpsilon);
  EXPECT_NEAR(normalized[2], 0.0, kEpsilon);
  EXPECT_NEAR(normalized.Magnitude(), 1.0, kEpsilon);
}

// 内積のテスト
TEST(VectorTest, DotProduct) {
  Vector<double, 3> v1(1.0, 2.0, 3.0);
  Vector<double, 3> v2(4.0, 5.0, 6.0);

  double dot = v1.Dot(v2);
  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
  EXPECT_NEAR(dot, 32.0, kEpsilon);
}

// 直交ベクトルの内積のテスト
TEST(VectorTest, DotProductOrthogonal) {
  Vector<double, 3> v1(1.0, 0.0, 0.0);
  Vector<double, 3> v2(0.0, 1.0, 0.0);

  double dot = v1.Dot(v2);
  EXPECT_NEAR(dot, 0.0, kEpsilon);
}

// 外積のテスト
TEST(VectorTest, CrossProduct) {
  Vector<double, 3> v1(1.0, 0.0, 0.0);
  Vector<double, 3> v2(0.0, 1.0, 0.0);
  auto cross = v1.Cross(v2);

  EXPECT_NEAR(cross[0], 0.0, kEpsilon);
  EXPECT_NEAR(cross[1], 0.0, kEpsilon);
  EXPECT_NEAR(cross[2], 1.0, kEpsilon);
}

// 外積の一般的なケースのテスト
TEST(VectorTest, CrossProductGeneral) {
  Vector<double, 3> v1(2.0, 3.0, 4.0);
  Vector<double, 3> v2(5.0, 6.0, 7.0);
  auto cross = v1.Cross(v2);

  // (3*7 - 4*6, 4*5 - 2*7, 2*6 - 3*5) = (21-24, 20-14, 12-15) = (-3, 6, -3)
  EXPECT_NEAR(cross[0], -3.0, kEpsilon);
  EXPECT_NEAR(cross[1], 6.0, kEpsilon);
  EXPECT_NEAR(cross[2], -3.0, kEpsilon);
}

// 外積の反可換性のテスト
TEST(VectorTest, CrossProductAnticommutative) {
  Vector<double, 3> v1(1.0, 2.0, 3.0);
  Vector<double, 3> v2(4.0, 5.0, 6.0);

  auto cross1 = v1.Cross(v2);
  auto cross2 = v2.Cross(v1);

  EXPECT_NEAR(cross1[0], -cross2[0], kEpsilon);
  EXPECT_NEAR(cross1[1], -cross2[1], kEpsilon);
  EXPECT_NEAR(cross1[2], -cross2[2], kEpsilon);
}

// ゼロベクトルのテスト
TEST(VectorTest, ZeroVector) {
  Vector<double, 3> zero;
  EXPECT_NEAR(zero.Magnitude(), 0.0, kEpsilon);
  EXPECT_DOUBLE_EQ(zero[0], 0.0);
  EXPECT_DOUBLE_EQ(zero[1], 0.0);
  EXPECT_DOUBLE_EQ(zero[2], 0.0);
}

// float型でのテスト
TEST(VectorTest, FloatType) {
  Vector<float, 2> vec(3.0f, 4.0f);
  EXPECT_NEAR(vec.Magnitude(), 5.0f, 1e-5f);
}

// int型でのテスト
TEST(VectorTest, IntegerType) {
  Vector<int, 2> vec(3, 4);
  auto sum = vec + Vector<int, 2>(1, 2);
  EXPECT_EQ(sum[0], 4);
  EXPECT_EQ(sum[1], 6);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
