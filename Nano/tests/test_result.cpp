#include <gtest/gtest.h>
#include <Nano/result.hpp>
#include <string>

using nano::utils::Err;
using nano::utils::Ok;
using nano::utils::Result;

// 基本的なOk結果のテスト
TEST(ResultTest, BasicOkValue) {
  auto result = []() -> Result<int, void*> {
    return Ok(42);
  }();

  EXPECT_TRUE(result.IsOk());
  ASSERT_TRUE(result.Value().has_value());
  EXPECT_EQ(result.Value().value(), 42);
  EXPECT_FALSE(result.Error().has_value());
}

// 基本的なError結果のテスト
TEST(ResultTest, BasicErrorValue) {
  Result<int, std::string> result = Err(std::string("error occurred"));

  EXPECT_FALSE(result.IsOk());
  EXPECT_FALSE(result.Value().has_value());
  ASSERT_TRUE(result.Error().has_value());
  EXPECT_EQ(result.Error().value(), "error occurred");
}

// 異なる型でのOkテスト
TEST(ResultTest, DifferentOkTypes) {
  Result<double, void*> floatResult = Ok(3.14);
  EXPECT_TRUE(floatResult.IsOk());
  EXPECT_DOUBLE_EQ(floatResult.Value().value(), 3.14);

  Result<std::string, int> stringResult = Ok(std::string("success"));
  EXPECT_TRUE(stringResult.IsOk());
  EXPECT_EQ(stringResult.Value().value(), "success");
}

// 構造体を値として持つResultのテスト
TEST(ResultTest, StructValue) {
  struct Point {
    int x, y;
    bool operator==(const Point& other) const {
      return x == other.x && y == other.y;
    }
  };

  Result<Point, void*> result = Ok(Point{10, 20});

  EXPECT_TRUE(result.IsOk());
  ASSERT_TRUE(result.Value().has_value());
  EXPECT_EQ(result.Value().value().x, 10);
  EXPECT_EQ(result.Value().value().y, 20);
}

// 構造体をエラーとして持つResultのテスト
TEST(ResultTest, StructError) {
  struct ErrorInfo {
    int code;
    std::string message;
  };

  Result<int, ErrorInfo> result = Err(ErrorInfo{500, "Internal Error"});

  EXPECT_FALSE(result.IsOk());
  ASSERT_TRUE(result.Error().has_value());
  EXPECT_EQ(result.Error().value().code, 500);
  EXPECT_EQ(result.Error().value().message, "Internal Error");
}

// コピーコンストラクタのテスト
TEST(ResultTest, CopyConstructor) {
  Result<int, std::string> original = Ok(100);
  Result<int, std::string> copy = original;

  EXPECT_TRUE(copy.IsOk());
  EXPECT_EQ(copy.Value().value(), 100);
}

// ムーブセマンティクスのテスト
TEST(ResultTest, MoveSemantics) {
  Result<std::string, int> original = Ok(std::string("movable string"));
  Result<std::string, int> moved = std::move(original);

  EXPECT_TRUE(moved.IsOk());
  EXPECT_EQ(moved.Value().value(), "movable string");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
