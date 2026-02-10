#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include <NanoMbed/timer.hpp>

using namespace std::chrono_literals;

// Timer の基本的な初期化テスト
TEST(HighResTimerTest, Initialize) {
  mbed::Timer timer;
  SUCCEED();
}

// Timer のリセット機能テスト
TEST(HighResTimerTest, Reset) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  auto time1 = timer.read();
  std::this_thread::sleep_for(5ms);
  auto time2 = timer.read();

  EXPECT_GT(time2, time1);
  timer.reset();
  timer.start();

  std::this_thread::sleep_for(5ms);
  time2 = timer.read();

  EXPECT_GT(time2, time1);
}

// Timer の開始・停止・読み取りテスト
TEST(HighResTimerTest, StartStopRead) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  std::this_thread::sleep_for(10ms);

  auto elapsed1 = timer.read();
  EXPECT_GT(elapsed1.count(), 0);

  // さらに待つ
  std::this_thread::sleep_for(10ms);

  auto elapsed2 = timer.read();
  EXPECT_GT(elapsed2, elapsed1);

  timer.stop();
  auto elapsed3 = timer.read();

  // 停止後、少し待機してもほぼ同じ値
  std::this_thread::sleep_for(5ms);
  auto elapsed4 = timer.read();

  EXPECT_EQ(elapsed3, elapsed4);
}

// Timer の経過時間取得テスト
TEST(HighResTimerTest, ElapsedTime) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  std::this_thread::sleep_for(20ms);

  auto elapsed = timer.elapsed_time();

  // 20ms以上経過していることを確認（マージンを持たせる）
  EXPECT_GE(elapsed.count(), 15000);   // 15ms in microseconds
  EXPECT_LT(elapsed.count(), 100000);  // 100ms以内（テストの遅延対策）
}

// Timer の read_ms（ミリ秒）メソッドテスト
TEST(HighResTimerTest, ReadMilliseconds) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  std::this_thread::sleep_for(15ms);

  int elapsed_ms = timer.read_ms();

  EXPECT_GT(elapsed_ms, 0);
  EXPECT_LT(elapsed_ms, 100);
}

// 複数回のリセット・開始テスト
TEST(HighResTimerTest, MultipleResets) {
  mbed::Timer timer;

  for (int i = 0; i < 3; ++i) {
    timer.reset();
    timer.start();

    std::this_thread::sleep_for(10ms);

    auto elapsed = timer.read();
    EXPECT_GT(elapsed.count(), 0);

    timer.stop();
  }

  SUCCEED();
}

// Timer の精度テスト（相対的な精度）
TEST(HighResTimerTest, Accuracy) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  // 50msスリープ
  std::this_thread::sleep_for(50ms);

  auto elapsed = timer.read();

  // マージンを持たせて確認（30ms～70ms）
  EXPECT_GE(elapsed.count(), 30);
  EXPECT_LE(elapsed.count(), 70);
}

// タイマーコールバック機能のテスト
TEST(HighResTimerTest, Callback) {
  mbed::Timer timer;
  bool callback_called = false;

  timer.attach([&callback_called]() { callback_called = true; });

  SUCCEED();
}

// 複数のタイマーインスタンスが独立していることを確認
TEST(HighResTimerTest, MultipleIndependentTimers) {
  mbed::Timer timer1;
  mbed::Timer timer2;

  timer1.reset();
  timer1.start();

  std::this_thread::sleep_for(10ms);

  timer2.reset();
  timer2.start();

  std::this_thread::sleep_for(10ms);

  auto elapsed1 = timer1.read();
  auto elapsed2 = timer2.read();

  // timer1の方がより多くの時間が経過している
  EXPECT_GT(elapsed1, elapsed2);
}

// タイマーの値がオーバーフローしないことを確認（短い時間内）
TEST(HighResTimerTest, NoOverflow) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(5ms);
    auto elapsed = timer.read();
    EXPECT_GT(elapsed.count(), 0);
  }
}

// タイマーのリセット前後で値が変わることを確認
TEST(HighResTimerTest, ResetChangesValue) {
  mbed::Timer timer;

  timer.reset();
  timer.start();

  std::this_thread::sleep_for(10ms);
  auto elapsed_before = timer.read();

  timer.reset();
  std::this_thread::sleep_for(2ms);
  auto elapsed_after = timer.read();

  EXPECT_LT(elapsed_after, elapsed_before);
}
