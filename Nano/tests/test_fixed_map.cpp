#include <gtest/gtest.h>
#include <Nano/fixed_map.hpp>
#include <string>

using nano::collection::FixedMap;

constexpr size_t kMapSize = 10;

// 基本的な挿入と検索のテスト
TEST(FixedMapTest, BasicInsertAndFind) {
  FixedMap<int, int, kMapSize> map;

  map[1] = 100;
  map[2] = 200;
  map[3] = 300;

  int* value1 = map.Find(1);
  ASSERT_NE(value1, nullptr);
  EXPECT_EQ(*value1, 100);

  int* value2 = map.Find(2);
  ASSERT_NE(value2, nullptr);
  EXPECT_EQ(*value2, 200);

  int* value3 = map.Find(3);
  ASSERT_NE(value3, nullptr);
  EXPECT_EQ(*value3, 300);
}

// 存在しないキーの検索テスト
TEST(FixedMapTest, FindNonExistent) {
  FixedMap<int, int, kMapSize> map;

  map[1] = 100;

  int* value = map.Find(999);
  EXPECT_EQ(value, nullptr);
}

// Contains メソッドのテスト
TEST(FixedMapTest, Contains) {
  FixedMap<int, int, kMapSize> map;

  map[10] = 1000;
  map[20] = 2000;

  EXPECT_TRUE(map.Contains(10));
  EXPECT_TRUE(map.Contains(20));
  EXPECT_FALSE(map.Contains(30));
  EXPECT_FALSE(map.Contains(0));
}

// operator[] で値を上書きするテスト
TEST(FixedMapTest, OverwriteValue) {
  FixedMap<int, int, kMapSize> map;

  map[1] = 100;  // NOLINT
  EXPECT_EQ(*map.Find(1), 100);

  map[1] = 999;
  EXPECT_EQ(*map.Find(1), 999);
}

// 文字列キーのテスト
TEST(FixedMapTest, StringKey) {
  FixedMap<std::string, int, kMapSize> map;

  map["one"] = 1;
  map["two"] = 2;    // NOLINT
  map["three"] = 3;  // NOLINT

  EXPECT_TRUE(map.Contains("one"));
  EXPECT_TRUE(map.Contains("two"));
  EXPECT_TRUE(map.Contains("three"));
  EXPECT_FALSE(map.Contains("four"));

  EXPECT_EQ(*map.Find("one"), 1);
  EXPECT_EQ(*map.Find("two"), 2);
  EXPECT_EQ(*map.Find("three"), 3);
}

// 構造体を値として格納するテスト
TEST(FixedMapTest, StructValue) {
  struct Data {
    int x;
    int y;
  };

  FixedMap<int, Data, kMapSize> map;

  map[1].x = 10;
  map[1].y = 20;

  map[2].x = 30;
  map[2].y = 40;

  Data* data1 = map.Find(1);
  ASSERT_NE(data1, nullptr);
  EXPECT_EQ(data1->x, 10);
  EXPECT_EQ(data1->y, 20);

  Data* data2 = map.Find(2);
  ASSERT_NE(data2, nullptr);
  EXPECT_EQ(data2->x, 30);
  EXPECT_EQ(data2->y, 40);
}

// イテレータの基本動作テスト
TEST(FixedMapTest, Iterator) {
  FixedMap<int, int, kMapSize> map;

  map[1] = 100;
  map[2] = 200;
  map[3] = 300;

  int valid_count = 0;
  for (auto& pair : map) {
    if (pair.is_valid) {
      valid_count++;
      EXPECT_NE(pair.value, nullptr);
    }
  }

  EXPECT_EQ(valid_count, 3);
}

// 空のマップのイテレータテスト
TEST(FixedMapTest, EmptyMapIterator) {
  FixedMap<int, int, kMapSize> map;

  int valid_count = 0;
  for (auto& pair : map) {
    if (pair.is_valid) {
      valid_count++;
    }
  }

  EXPECT_EQ(valid_count, 0);
}

// マップが満杯になるまで挿入するテスト
TEST(FixedMapTest, FillMap) {
  FixedMap<int, int, kMapSize> map;

  for (int i = 0; i < static_cast<int>(kMapSize); ++i) {
    map[i] = i * 10;
  }

  for (int i = 0; i < static_cast<int>(kMapSize); ++i) {
    EXPECT_TRUE(map.Contains(i));
    EXPECT_EQ(*map.Find(i), i * 10);
  }
}

// 同じキーに複数回アクセスするテスト
TEST(FixedMapTest, MultipleAccessSameKey) {
  FixedMap<int, int, kMapSize> map;

  map[5] = 50;

  int* ptr1 = map.Find(5);
  int* ptr2 = map.Find(5);
  int& ref1 = map[5];
  int& ref2 = map[5];

  ASSERT_NE(ptr1, nullptr);
  ASSERT_NE(ptr2, nullptr);
  EXPECT_EQ(ptr1, ptr2);
  EXPECT_EQ(&ref1, &ref2);
  EXPECT_EQ(*ptr1, 50);
}

// operator[] での新規作成テスト
TEST(FixedMapTest, OperatorBracketCreatesNewEntry) {
  FixedMap<int, int, kMapSize> map;

  EXPECT_FALSE(map.Contains(42));

  int& value = map[42];
  value = 4200;

  EXPECT_TRUE(map.Contains(42));
  EXPECT_EQ(*map.Find(42), 4200);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
