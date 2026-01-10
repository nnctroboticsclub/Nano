#include <gtest/gtest.h>
#include <Nano/angle.hpp>
#include <cmath>

using namespace robotics::types;

constexpr double kEpsilon = 1e-6;

// 基本的な角度の作成と取得のテスト
TEST(AngleTest, BasicCreation) {
  Angle<double, AngleType::kDegrees> deg(90.0);
  EXPECT_DOUBLE_EQ(deg.value, 90.0);

  Angle<double, AngleType::kRadians> rad(M_PI / 2);
  EXPECT_DOUBLE_EQ(rad.value, M_PI / 2);
}

// デフォルトコンストラクタのテスト
TEST(AngleTest, DefaultConstructor) {
  Angle<double, AngleType::kDegrees> deg;
  EXPECT_DOUBLE_EQ(deg.value, 0.0);

  Angle<double, AngleType::kRadians> rad;
  EXPECT_DOUBLE_EQ(rad.value, 0.0);
}

// 度からラジアンへの変換テスト
TEST(AngleTest, DegreesToRadians) {
  Angle<double, AngleType::kDegrees> deg(180.0);
  auto rad = deg.ToRadians();
  EXPECT_NEAR(rad.value, M_PI, kEpsilon);

  Angle<double, AngleType::kDegrees> deg90(90.0);
  auto rad90 = deg90.ToRadians();
  EXPECT_NEAR(rad90.value, M_PI / 2, kEpsilon);

  Angle<double, AngleType::kDegrees> deg360(360.0);
  auto rad360 = deg360.ToRadians();
  EXPECT_NEAR(rad360.value, 2 * M_PI, kEpsilon);
}

// ラジアンから度への変換テスト
TEST(AngleTest, RadiansToDegrees) {
  Angle<double, AngleType::kRadians> rad(M_PI);
  auto deg = rad.ToDegrees();
  EXPECT_NEAR(deg.value, 180.0, kEpsilon);

  Angle<double, AngleType::kRadians> rad90(M_PI / 2);
  auto deg90 = rad90.ToDegrees();
  EXPECT_NEAR(deg90.value, 90.0, kEpsilon);

  Angle<double, AngleType::kRadians> rad360(2 * M_PI);
  auto deg360 = rad360.ToDegrees();
  EXPECT_NEAR(deg360.value, 360.0, kEpsilon);
}

// 加算演算子のテスト
TEST(AngleTest, Addition) {
  Angle<double, AngleType::kDegrees> deg1(45.0);
  Angle<double, AngleType::kDegrees> deg2(30.0);
  auto result = deg1 + deg2;
  EXPECT_DOUBLE_EQ(result.value, 75.0);

  Angle<double, AngleType::kRadians> rad1(M_PI / 4);
  Angle<double, AngleType::kRadians> rad2(M_PI / 6);
  auto radResult = rad1 + rad2;
  EXPECT_NEAR(radResult.value, M_PI / 4 + M_PI / 6, kEpsilon);
}

// 減算演算子のテスト
TEST(AngleTest, Subtraction) {
  Angle<double, AngleType::kDegrees> deg1(90.0);
  Angle<double, AngleType::kDegrees> deg2(30.0);
  auto result = deg1 - deg2;
  EXPECT_DOUBLE_EQ(result.value, 60.0);

  Angle<double, AngleType::kRadians> rad1(M_PI);
  Angle<double, AngleType::kRadians> rad2(M_PI / 2);
  auto radResult = rad1 - rad2;
  EXPECT_NEAR(radResult.value, M_PI / 2, kEpsilon);
}

// 乗算演算子のテスト
TEST(AngleTest, Multiplication) {
  Angle<double, AngleType::kDegrees> deg(45.0);
  auto result = deg * 2.0;
  EXPECT_DOUBLE_EQ(result.value, 90.0);

  Angle<double, AngleType::kRadians> rad(M_PI / 4);
  auto radResult = rad * 4.0;
  EXPECT_NEAR(radResult.value, M_PI, kEpsilon);
}

// 除算演算子のテスト
TEST(AngleTest, Division) {
  Angle<double, AngleType::kDegrees> deg(180.0);
  auto result = deg / 2.0;
  EXPECT_DOUBLE_EQ(result.value, 90.0);

  Angle<double, AngleType::kRadians> rad(M_PI);
  auto radResult = rad / 2.0;
  EXPECT_NEAR(radResult.value, M_PI / 2, kEpsilon);
}

// 複合代入演算子のテスト
TEST(AngleTest, CompoundAssignment) {
  Angle<double, AngleType::kDegrees> deg(45.0);

  deg += Angle<double, AngleType::kDegrees>(15.0);
  EXPECT_DOUBLE_EQ(deg.value, 60.0);

  deg -= Angle<double, AngleType::kDegrees>(10.0);
  EXPECT_DOUBLE_EQ(deg.value, 50.0);

  deg *= 2.0;
  EXPECT_DOUBLE_EQ(deg.value, 100.0);

  deg /= 4.0;
  EXPECT_DOUBLE_EQ(deg.value, 25.0);
}

// 負の角度のテスト
TEST(AngleTest, NegativeAngles) {
  Angle<double, AngleType::kDegrees> deg(-90.0);
  EXPECT_DOUBLE_EQ(deg.value, -90.0);

  auto rad = deg.ToRadians();
  EXPECT_NEAR(rad.value, -M_PI / 2, kEpsilon);

  auto degBack = rad.ToDegrees();
  EXPECT_NEAR(degBack.value, -90.0, kEpsilon);
}

// float型での動作テスト
TEST(AngleTest, FloatType) {
  Angle<float, AngleType::kDegrees> deg(90.0f);
  auto rad = deg.ToRadians();
  EXPECT_NEAR(rad.value, static_cast<float>(M_PI) / 2.0f, 1e-5f);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
