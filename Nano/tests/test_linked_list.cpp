#include <gtest/gtest.h>
#include <Nano/linked_list.hpp>

using nano::collection::LinkedList;
using nano::collection::LinkedListNode;

constexpr size_t kListSize = 10;

struct TestNode : public LinkedListNode<TestNode> {
  int data;
};

// 基本的なノード作成のテスト
TEST(LinkedListTest, BasicNewNode) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node = list.NewNode();
  ASSERT_NE(node, nullptr);
  EXPECT_TRUE(node->InUse());
}

// 複数のノード作成テスト
TEST(LinkedListTest, MultipleNewNode) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewNode();
  TestNode* node2 = list.NewNode();
  TestNode* node3 = list.NewNode();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);
  ASSERT_NE(node3, nullptr);

  EXPECT_NE(node1, node2);
  EXPECT_NE(node2, node3);
  EXPECT_NE(node1, node3);

  EXPECT_TRUE(node1->InUse());
  EXPECT_TRUE(node2->InUse());
  EXPECT_TRUE(node3->InUse());
}

// プールが満杯になるテスト
TEST(LinkedListTest, PoolFull) {
  LinkedList<TestNode, kListSize> list;

  TestNode* nodes[kListSize];
  for (size_t i = 0; i < kListSize; ++i) {
    nodes[i] = list.NewNode();
    ASSERT_NE(nodes[i], nullptr);
  }

  TestNode* overflow = list.NewNode();
  EXPECT_EQ(overflow, nullptr);
}

// NewBack で末尾にノードを追加するテスト
TEST(LinkedListTest, NewBack) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewBack();
  ASSERT_NE(node1, nullptr);
  node1->data = 100;

  EXPECT_EQ(list.head_node(), node1);
  EXPECT_EQ(list.tail_node(), node1);

  TestNode* node2 = list.NewBack();
  ASSERT_NE(node2, nullptr);
  node2->data = 200;

  EXPECT_EQ(list.head_node(), node1);
  EXPECT_EQ(list.tail_node(), node2);
  EXPECT_EQ(node1->Next(), node2);
  EXPECT_EQ(node2->Prev(), node1);
}

// 複数の NewBack 呼び出しでリンクが正しく構築されるテスト
TEST(LinkedListTest, MultipleNewBack) {
  LinkedList<TestNode, kListSize> list;

  constexpr int kNodeCount = 5;
  TestNode* nodes[kNodeCount];

  for (int i = 0; i < kNodeCount; ++i) {
    nodes[i] = list.NewBack();
    ASSERT_NE(nodes[i], nullptr);
    nodes[i]->data = i * 10;
  }

  // 先頭から順に確認
  EXPECT_EQ(list.head_node(), nodes[0]);
  for (int i = 0; i < kNodeCount - 1; ++i) {
    EXPECT_EQ(nodes[i]->Next(), nodes[i + 1]);
  }

  // 末尾から順に確認
  EXPECT_EQ(list.tail_node(), nodes[kNodeCount - 1]);
  for (int i = kNodeCount - 1; i > 0; --i) {
    EXPECT_EQ(nodes[i]->Prev(), nodes[i - 1]);
  }
}

// イテレータの基本動作テスト
TEST(LinkedListTest, Iterator) {
  LinkedList<TestNode, kListSize> list;

  constexpr int kNodeCount = 5;
  for (int i = 0; i < kNodeCount; ++i) {
    TestNode* node = list.NewBack();
    ASSERT_NE(node, nullptr);
    node->data = i * 10;
  }

  int expected_data = 0;
  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    TestNode* node = *it;
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data, expected_data);
    expected_data += 10;
    count++;
  }

  EXPECT_EQ(count, kNodeCount);
}

// 空のリストのイテレータテスト
TEST(LinkedListTest, EmptyListIterator) {
  LinkedList<TestNode, kListSize> list;

  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    count++;
  }

  EXPECT_EQ(count, 0);
}

// Clear でリストをクリアするテスト
TEST(LinkedListTest, Clear) {
  LinkedList<TestNode, kListSize> list;

  for (int i = 0; i < 5; ++i) {
    TestNode* node = list.NewBack();
    ASSERT_NE(node, nullptr);
    node->data = i * 10;
  }

  list.Clear();

  EXPECT_EQ(list.head_node(), nullptr);
  EXPECT_EQ(list.tail_node(), nullptr);

  int count = 0;
  for (auto it = list.begin(); it != list.end(); ++it) {
    count++;
  }
  EXPECT_EQ(count, 0);

  // クリア後に再度ノードを作成できるか確認
  for (size_t i = 0; i < kListSize; ++i) {
    TestNode* node = list.NewNode();
    ASSERT_NE(node, nullptr);
  }
}

// Remove でノードを削除するテスト(先頭を削除)
TEST(LinkedListTest, RemoveHead) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewBack();
  TestNode* node2 = list.NewBack();
  TestNode* node3 = list.NewBack();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);
  ASSERT_NE(node3, nullptr);

  list.Remove(node1);

  EXPECT_FALSE(node1->InUse());
  EXPECT_EQ(list.head_node(), node2);
  EXPECT_EQ(list.tail_node(), node3);
}

// Remove でノードを削除するテスト(末尾を削除)
TEST(LinkedListTest, RemoveTail) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewBack();
  TestNode* node2 = list.NewBack();
  TestNode* node3 = list.NewBack();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);
  ASSERT_NE(node3, nullptr);

  list.Remove(node3);

  EXPECT_FALSE(node3->InUse());
  EXPECT_EQ(list.head_node(), node1);
  EXPECT_EQ(list.tail_node(), node2);
}

// Remove でノードを削除するテスト(中間を削除)
TEST(LinkedListTest, RemoveMiddle) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewBack();
  TestNode* node2 = list.NewBack();
  TestNode* node3 = list.NewBack();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);
  ASSERT_NE(node3, nullptr);

  list.Remove(node2);

  EXPECT_FALSE(node2->InUse());
  EXPECT_EQ(list.head_node(), node1);
  EXPECT_EQ(list.tail_node(), node3);
  EXPECT_EQ(node1->Next(), node3);
  EXPECT_EQ(node3->Prev(), node1);
}

// InUse のテスト
TEST(LinkedListTest, InUseStatus) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewNode();
  TestNode* node2 = list.NewNode();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);

  EXPECT_TRUE(node1->InUse());
  EXPECT_TRUE(node2->InUse());

  node1->InUse(false);
  EXPECT_FALSE(node1->InUse());
  EXPECT_TRUE(node2->InUse());
}

// ResetLinkNode のテスト
TEST(LinkedListTest, ResetLinkNode) {
  LinkedList<TestNode, kListSize> list;

  TestNode* node1 = list.NewBack();
  TestNode* node2 = list.NewBack();

  ASSERT_NE(node1, nullptr);
  ASSERT_NE(node2, nullptr);

  EXPECT_EQ(node1->Next(), node2);
  EXPECT_EQ(node2->Prev(), node1);

  node1->ResetLinkNode();

  // Note: リンクノードがリセットされても、リスト構造は維持される
  // ResetLinkNode は個別ノードのポインタをクリアするだけ
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
