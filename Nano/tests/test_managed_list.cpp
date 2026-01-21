#include <gtest/gtest.h>
#include <Nano/managed_list.hpp>

using Nano::collection::ManagedList;

constexpr size_t kPoolSize = 10;

// 基本的な New() の動作テスト
TEST(ManagedListTest, BasicNew) {
  ManagedList<int, kPoolSize> list;

  int* ptr1 = list.New();
  ASSERT_NE(ptr1, nullptr);

  *ptr1 = 42;
  EXPECT_EQ(*ptr1, 42);
}

// 複数回の New() 呼び出しテスト
TEST(ManagedListTest, MultipleNew) {
  ManagedList<int, kPoolSize> list;

  int* ptrs[5];
  for (int i = 0; i < 5; ++i) {
    ptrs[i] = list.New();
    ASSERT_NE(ptrs[i], nullptr);
    *ptrs[i] = i * 10;
  }

  // 値が正しく保存されているか確認
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(*ptrs[i], i * 10);
  }
}

// プールが満杯になった時の動作テスト
TEST(ManagedListTest, PoolFull) {
  ManagedList<int, kPoolSize> list;

  // プールサイズまで確保
  for (size_t i = 0; i < kPoolSize; ++i) {
    int* ptr = list.New();
    ASSERT_NE(ptr, nullptr) << "Failed at index " << i;
  }

  // プールが満杯なので nullptr が返される
  int* overflow_ptr = list.New();
  EXPECT_EQ(overflow_ptr, nullptr);
}

// イテレータの動作テスト
TEST(ManagedListTest, Iterator) {
  ManagedList<int, kPoolSize> list;

  // いくつか要素を追加
  for (int i = 0; i < 5; ++i) {
    int* ptr = list.New();
    *ptr = i;
  }

  // イテレータでアクセス
  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    count++;
  }

  EXPECT_EQ(count, kPoolSize);  // プール全体を反復
}

// 構造体を格納するテスト
TEST(ManagedListTest, StructStorage) {
  struct Point {
    int x, y;
  };

  ManagedList<Point, kPoolSize> list;

  Point* p1 = list.New();
  ASSERT_NE(p1, nullptr);
  p1->x = 10;
  p1->y = 20;

  Point* p2 = list.New();
  ASSERT_NE(p2, nullptr);
  p2->x = 30;
  p2->y = 40;

  EXPECT_EQ(p1->x, 10);
  EXPECT_EQ(p1->y, 20);
  EXPECT_EQ(p2->x, 30);
  EXPECT_EQ(p2->y, 40);
}

// New() の連続呼び出しで異なるポインタが返されるか
TEST(ManagedListTest, UniquePointers) {
  ManagedList<int, kPoolSize> list;

  int* ptr1 = list.New();
  int* ptr2 = list.New();
  int* ptr3 = list.New();

  ASSERT_NE(ptr1, nullptr);
  ASSERT_NE(ptr2, nullptr);
  ASSERT_NE(ptr3, nullptr);

  EXPECT_NE(ptr1, ptr2);
  EXPECT_NE(ptr2, ptr3);
  EXPECT_NE(ptr1, ptr3);
}

// 空のリストのイテレータテスト
TEST(ManagedListTest, EmptyListIterator) {
  ManagedList<int, kPoolSize> list;

  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    count++;
  }

  EXPECT_EQ(count, kPoolSize);  // 初期化されたプール全体を反復
}

// プールサイズ 1 の動作テスト
TEST(ManagedListTest, SingleElementPool) {
  ManagedList<int, 1> list;

  int* ptr1 = list.New();
  ASSERT_NE(ptr1, nullptr);
  *ptr1 = 100;

  int* ptr2 = list.New();
  EXPECT_EQ(ptr2, nullptr);  // 満杯

  EXPECT_EQ(*ptr1, 100);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
