#include <gtest/gtest.h>

#include <thread>

#include <NanoMbed/high_res_clock.hpp>
#include "NanoHW/high_res_clock_impl.hpp"
#include "high_res_clock.hpp"

using namespace std::chrono_literals;

template struct nano_hw::HighResClockImpl<nano_stub::StubHighResClock>;

// HighResClock の基本的なテスト
TEST(HighResClockTest, TimeProgresses) {
  using Clock = mbed::HighResClock;

  auto time1 = Clock::now();
  std::this_thread::sleep_for(10ms);
  auto time2 = Clock::now();

  EXPECT_GT(time2, time1);
}

// HighResClock が単調増加することを確認
TEST(HighResClockTest, MonotonicProgress) {
  using Clock = mbed::HighResClock;

  auto time1 = Clock::now();
  auto time2 = Clock::now();
  auto time3 = Clock::now();

  EXPECT_LE(time1, time2);
  EXPECT_LE(time2, time3);
}
