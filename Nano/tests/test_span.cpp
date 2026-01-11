#include <gtest/gtest.h>
#include <Nano/span.hpp>

using nano::collection::Span;

// Span の基本的なコンストラクタテスト
TEST(SpanTest, BasicConstruction) {
  int data[] = {1, 2, 3, 4, 5};
  Span<int> span(data, 5);

  EXPECT_EQ(span.data(), data);
  EXPECT_EQ(span.size(), 5);
}

// デフォルトコンストラクタのテスト
TEST(SpanTest, DefaultConstruction) {
  Span<int> span;

  EXPECT_EQ(span.data(), nullptr);
  EXPECT_EQ(span.size(), 0);
}

// 要素へのアクセステスト
TEST(SpanTest, ElementAccess) {
  int data[] = {10, 20, 30, 40, 50};
  Span<int> span(data, 5);

  EXPECT_EQ(span[0], 10);
  EXPECT_EQ(span[1], 20);
  EXPECT_EQ(span[2], 30);
  EXPECT_EQ(span[3], 40);
  EXPECT_EQ(span[4], 50);
}

// 要素の変更テスト
TEST(SpanTest, ElementModification) {
  int data[] = {1, 2, 3};
  Span<int> span(data, 3);

  span[0] = 100;
  span[1] = 200;
  span[2] = 300;

  EXPECT_EQ(data[0], 100);
  EXPECT_EQ(data[1], 200);
  EXPECT_EQ(data[2], 300);
}

// コピーコンストラクタのテスト
TEST(SpanTest, CopyConstruction) {
  int data[] = {1, 2, 3, 4};
  Span<int> span1(data, 4);
  Span<int> span2(span1);

  EXPECT_EQ(span2.data(), span1.data());
  EXPECT_EQ(span2.size(), span1.size());
  EXPECT_EQ(span2[0], 1);
  EXPECT_EQ(span2[3], 4);
}

// const データを持つ Span のテスト
TEST(SpanTest, ConstData) {
  const int data[] = {5, 10, 15};
  Span<const int> span(data, 3);

  EXPECT_EQ(span.data(), data);
  EXPECT_EQ(span.size(), 3);
  EXPECT_EQ(span[0], 5);
  EXPECT_EQ(span[1], 10);
  EXPECT_EQ(span[2], 15);
}

// 空の Span のテスト
TEST(SpanTest, EmptySpan) {
  int data[] = {1};
  Span<int> span(data, 0);

  EXPECT_EQ(span.size(), 0);
}

// size が正しく返されるかのテスト
TEST(SpanTest, SizeCheck) {
  int data[100];
  Span<int> span(data, 100);

  EXPECT_EQ(span.size(), 100);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
