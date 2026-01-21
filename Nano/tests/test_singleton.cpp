#include <gtest/gtest.h>
#include <Nano/singleton.hpp>

using Nano::utils::Singleton;

// テスト用のシングルトンクラス
class TestSingleton : public Singleton<TestSingleton> {
  friend class Singleton<TestSingleton>;

 private:
  int value_ = 0;
  TestSingleton() = default;

 public:
  void SetValue(int value) { value_ = value; }
  int GetValue() const { return value_; }
};

// 別のテスト用シングルトン
class AnotherSingleton : public Singleton<AnotherSingleton> {
  friend class Singleton<AnotherSingleton>;

 private:
  std::string name_{"default"};

 public:
  void SetName(const std::string& name) { name_ = name; }
  const std::string& GetName() const { return name_; }
};

// 別のテスト用シングルトン
class DefaultConstructTest : public Singleton<DefaultConstructTest> {
  friend class Singleton<DefaultConstructTest>;

 private:
  std::string name_{"default"};

 public:
  void SetName(const std::string& name) { name_ = name; }
  const std::string& GetName() const { return name_; }
};

// インスタンスが同一であることを確認
TEST(SingletonTest, SameInstance) {
  auto& instance1 = TestSingleton::GetInstance();
  auto& instance2 = TestSingleton::GetInstance();

  EXPECT_EQ(&instance1, &instance2);
}

// 状態が共有されることを確認
TEST(SingletonTest, SharedState) {
  auto& instance1 = TestSingleton::GetInstance();
  instance1.SetValue(42);

  auto& instance2 = TestSingleton::GetInstance();
  EXPECT_EQ(instance2.GetValue(), 42);

  instance2.SetValue(100);
  EXPECT_EQ(instance1.GetValue(), 100);
}

// 異なる型のシングルトンは独立していることを確認
TEST(SingletonTest, IndependentTypes) {
  auto& test_instance = TestSingleton::GetInstance();
  auto& another_instance = AnotherSingleton::GetInstance();

  test_instance.SetValue(10);
  another_instance.SetName("test");

  EXPECT_EQ(test_instance.GetValue(), 10);
  EXPECT_EQ(another_instance.GetName(), "test");

  // アドレスが異なることを確認
  EXPECT_NE(static_cast<void*>(&test_instance),
            static_cast<void*>(&another_instance));
}

// 複数回呼び出しても同じインスタンスが返されることを確認
TEST(SingletonTest, MultipleGetInstance) {
  for (int i = 0; i < 100; ++i) {
    auto& instance = TestSingleton::GetInstance();
    instance.SetValue(i);
  }

  auto& final_instance = TestSingleton::GetInstance();
  EXPECT_EQ(final_instance.GetValue(), 99);
}

// デフォルト構築された状態を確認
TEST(SingletonTest, DefaultConstruction) {
  // 新しいシングルトンを取得して初期状態を確認
  auto& instance = DefaultConstructTest::GetInstance();
  EXPECT_EQ(instance.GetName(), "default");
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
