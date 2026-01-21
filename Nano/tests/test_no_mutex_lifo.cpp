#include <gtest/gtest.h>
#include <Nano/no_mutex_lifo.hpp>

using Nano::collection::NoMutexLIFO;

constexpr size_t kBufferSize = 8;

// 基本的な Push/Pop のテスト
TEST(NoMutexLIFOTest, BasicPushPop) {
  NoMutexLIFO<int, kBufferSize> lifo;

  EXPECT_TRUE(lifo.Empty());
  EXPECT_EQ(lifo.Size(), 0);
  EXPECT_EQ(lifo.Capacity(), kBufferSize);

  // Push
  EXPECT_TRUE(lifo.Push(1));
  EXPECT_FALSE(lifo.Empty());
  EXPECT_EQ(lifo.Size(), 1);

  EXPECT_TRUE(lifo.Push(2));
  EXPECT_EQ(lifo.Size(), 2);

  EXPECT_TRUE(lifo.Push(3));
  EXPECT_EQ(lifo.Size(), 3);
  // Pop (LIFO: Last In First Out)
  EXPECT_EQ(lifo.Pop(), 1);
  EXPECT_EQ(lifo.Size(), 2);

  EXPECT_EQ(lifo.Pop(), 2);
  EXPECT_EQ(lifo.Size(), 1);

  EXPECT_EQ(lifo.Pop(), 3);
  EXPECT_EQ(lifo.Size(), 0);
  EXPECT_TRUE(lifo.Empty());
}

// バッファがフルになるまで Push するテスト
TEST(NoMutexLIFOTest, FillBuffer) {
  NoMutexLIFO<int, kBufferSize> lifo;

  // 7個まで Push できる (N-1個)
  for (int i = 0; i < kBufferSize - 1; ++i) {
    EXPECT_TRUE(lifo.Push(i)) << "Failed to push " << i;
    EXPECT_EQ(lifo.Size(), i + 1);
  }

  EXPECT_TRUE(lifo.Full());

  // フルの状態では Push できない
  EXPECT_FALSE(lifo.Push(999));
  EXPECT_EQ(lifo.Size(), 7);

  // Pop して確認
  for (int i = 0; i < kBufferSize - 1; ++i) {
    EXPECT_EQ(lifo.Pop(), i);
  }

  EXPECT_TRUE(lifo.Empty());
}

// Clear のテスト
TEST(NoMutexLIFOTest, Clear) {
  NoMutexLIFO<int, kBufferSize> lifo;

  lifo.Push(1);
  lifo.Push(2);
  lifo.Push(3);
  EXPECT_EQ(lifo.Size(), 3);

  lifo.Clear();
  EXPECT_TRUE(lifo.Empty());
  EXPECT_EQ(lifo.Size(), 0);
}

// 空の状態で Pop するテスト
TEST(NoMutexLIFOTest, PopFromEmpty) {
  NoMutexLIFO<int, kBufferSize> lifo;

  EXPECT_TRUE(lifo.Empty());
  EXPECT_EQ(lifo.Pop(), 0);  // デフォルト値が返される
  EXPECT_TRUE(lifo.Empty());
}

// PushN のテスト
TEST(NoMutexLIFOTest, PushN) {
  NoMutexLIFO<int, kBufferSize> lifo;

  std::array<int, 4> data{10, 20, 30, 40};  // NOLINT
  EXPECT_TRUE(lifo.PushN(data.data(), 4));
  EXPECT_EQ(lifo.Size(), 4);

  EXPECT_EQ(lifo.Pop(), 10);
  EXPECT_EQ(lifo.Pop(), 20);
  EXPECT_EQ(lifo.Pop(), 30);
  EXPECT_EQ(lifo.Pop(), 40);
}

// PushN でキャパシティを超える場合のテスト
TEST(NoMutexLIFOTest, PushNOverCapacity) {
  NoMutexLIFO<int, kBufferSize> lifo;

  std::array<int, kBufferSize + 1> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  // 8個は入る（Capacity()が8なので）
  EXPECT_TRUE(lifo.PushN(data.data(), 7));
  EXPECT_EQ(lifo.Size(), 7);

  // 既に7個入っているので、2個以上は入らない
  lifo.Clear();
  EXPECT_TRUE(lifo.PushN(data.data(), 7));
  int extra[2] = {10, 11};
  EXPECT_FALSE(lifo.PushN(extra, 2));  // 7+2 > 7 (最大サイズ)

  // 9個は確実に入らない
  lifo.Clear();
  EXPECT_FALSE(lifo.PushN(data.data(), 9));
  EXPECT_EQ(lifo.Size(), 0);
}

// PopNTo のテスト
TEST(NoMutexLIFOTest, PopNTo) {
  NoMutexLIFO<int, kBufferSize> lifo;

  int push_data[] = {10, 20, 30, 40, 50};
  lifo.PushN(push_data, 5);

  int pop_data[3] = {0};
  lifo.PopNTo(3, pop_data);

  EXPECT_EQ(pop_data[0], 10);
  EXPECT_EQ(pop_data[1], 20);
  EXPECT_EQ(pop_data[2], 30);
  EXPECT_EQ(lifo.Size(), 2);

  EXPECT_EQ(lifo.Pop(), 40);
  EXPECT_EQ(lifo.Pop(), 50);
}

// PopAllTo のテスト
TEST(NoMutexLIFOTest, PopAllTo) {
  NoMutexLIFO<int, kBufferSize> lifo;

  int push_data[] = {100, 200, 300, 400};
  lifo.PushN(push_data, 4);

  int pop_data[4] = {0};
  size_t popped = lifo.PopAllTo(pop_data);

  EXPECT_EQ(popped, 4);
  EXPECT_EQ(pop_data[0], 100);
  EXPECT_EQ(pop_data[1], 200);
  EXPECT_EQ(pop_data[2], 300);
  EXPECT_EQ(pop_data[3], 400);
  EXPECT_TRUE(lifo.Empty());
}

// ConsumeN のテスト
TEST(NoMutexLIFOTest, ConsumeN) {
  NoMutexLIFO<int, kBufferSize> lifo;

  for (int i = 0; i < 5; ++i) {
    lifo.Push(i);
  }

  EXPECT_EQ(lifo.Size(), 5);

  lifo.ConsumeN(2);
  EXPECT_EQ(lifo.Size(), 3);

  // 残りの要素を確認
  EXPECT_EQ(lifo.Pop(), 2);
  EXPECT_EQ(lifo.Pop(), 3);
  EXPECT_EQ(lifo.Pop(), 4);
}

// operator[] のテスト
TEST(NoMutexLIFOTest, OperatorBracket) {
  NoMutexLIFO<int, kBufferSize> lifo;

  lifo.Push(10);
  lifo.Push(20);
  lifo.Push(30);

  EXPECT_EQ(lifo[0], 10);
  EXPECT_EQ(lifo[1], 20);
  EXPECT_EQ(lifo[2], 30);

  // 値を変更
  lifo[1] = 999;
  EXPECT_EQ(lifo[1], 999);

  // Pop で確認
  EXPECT_EQ(lifo.Pop(), 10);
  EXPECT_EQ(lifo.Pop(), 999);
  EXPECT_EQ(lifo.Pop(), 30);
}

// リングバッファのラップアラウンドテスト
TEST(NoMutexLIFOTest, WrapAround) {
  NoMutexLIFO<int, kBufferSize> lifo;

  // バッファをほぼ埋める
  for (int i = 0; i < kBufferSize - 1; ++i) {
    lifo.Push(i);
  }

  // いくつか Pop
  for (int i = 0; i < 3; ++i) {
    lifo.Pop();
  }

  // 再度 Push（ラップアラウンドが発生）
  EXPECT_TRUE(lifo.Push(100));
  EXPECT_TRUE(lifo.Push(200));
  EXPECT_TRUE(lifo.Push(300));

  EXPECT_EQ(lifo.Size(), 7);

  // 順番を確認
  EXPECT_EQ(lifo.Pop(), 3);
  EXPECT_EQ(lifo.Pop(), 4);
  EXPECT_EQ(lifo.Pop(), 5);
  EXPECT_EQ(lifo.Pop(), 6);
  EXPECT_EQ(lifo.Pop(), 100);
  EXPECT_EQ(lifo.Pop(), 200);
  EXPECT_EQ(lifo.Pop(), 300);
}

// 異なる型のテスト
TEST(NoMutexLIFOTest, DifferentTypes) {
  NoMutexLIFO<double, 5> lifo;

  EXPECT_TRUE(lifo.Push(1.5));
  EXPECT_TRUE(lifo.Push(2.7));
  EXPECT_TRUE(lifo.Push(3.14));

  EXPECT_DOUBLE_EQ(lifo.Pop(), 1.5);
  EXPECT_DOUBLE_EQ(lifo.Pop(), 2.7);
  EXPECT_DOUBLE_EQ(lifo.Pop(), 3.14);
}

// 構造体のテスト
TEST(NoMutexLIFOTest, StructType) {
  struct Point {
    int x, y;
    bool operator==(const Point& other) const {
      return x == other.x && y == other.y;
    }
  };

  NoMutexLIFO<Point, kBufferSize> lifo;

  Point p1{1, 2};
  Point p2{3, 4};
  Point p3{5, 6};

  EXPECT_TRUE(lifo.Push(p1));
  EXPECT_TRUE(lifo.Push(p2));
  EXPECT_TRUE(lifo.Push(p3));

  EXPECT_EQ(lifo.Pop(), p1);
  EXPECT_EQ(lifo.Pop(), p2);
  EXPECT_EQ(lifo.Pop(), p3);
}

// PushN と PopNTo のラップアラウンドテスト
TEST(NoMutexLIFOTest, PushNPopNToWrapAround) {
  NoMutexLIFO<int, kBufferSize> lifo;

  // バッファの後半に配置
  for (int i = 0; i < 5; ++i) {
    lifo.Push(i);
  }
  for (int i = 0; i < 5; ++i) {
    lifo.Pop();
  }

  // ラップアラウンドする PushN
  int push_data[] = {10, 20, 30, 40, 50};
  EXPECT_TRUE(lifo.PushN(push_data, 5));
  EXPECT_EQ(lifo.Size(), 5);

  // PopNTo で取得
  int pop_data[5] = {0};
  lifo.PopNTo(5, pop_data);

  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(pop_data[i], push_data[i]);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
