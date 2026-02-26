#pragma once
#include "NanoHW/timer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace nano_stub {

template <nano_hw::timer::TimerConfig Config>
class MockTimer {
 public:
  MockTimer()
      : start_time_(std::chrono::steady_clock::now()), accumulated_time_(0) {
    std::cout << "MockTimer initialized\n";
  }

  void Reset() {
    std::cout << "Timer Reset called\n";
    accumulated_time_ = std::chrono::milliseconds(0);
    start_time_ = std::chrono::steady_clock::now();
  }

  void Start() {
    std::cout << "Timer Start called\n";
    if (!is_running_) {
      start_time_ = std::chrono::steady_clock::now();
      is_running_ = true;
    }
  }

  void Stop() {
    std::cout << "Timer Stop called\n";
    if (is_running_) {
      auto now = std::chrono::steady_clock::now();
      accumulated_time_ +=
          std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                                start_time_);
      is_running_ = false;
    }
  }

  std::chrono::milliseconds Read() {
    if (is_running_) {
      auto now = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
          now - start_time_);
      return accumulated_time_ + elapsed;
    }
    return accumulated_time_;
  }

  bool EnableTick(std::chrono::milliseconds interval) {
    std::cout << "EnableTick called with interval: " << interval.count()
              << " ms\n";
    tick_thread_ = std::thread([this, interval]() {
      while (true) {
        std::this_thread::sleep_for(interval);
        Config::OnTick::execute(this);
      }
    });
    tick_thread_->detach();
    return true;
  }

 private:
  std::chrono::steady_clock::time_point start_time_;
  bool is_running_ = false;
  std::chrono::milliseconds accumulated_time_;
  std::optional<std::thread> tick_thread_ = std::nullopt;
};

static_assert(nano_hw::timer::Timer<MockTimer>);

}  // namespace nano_stub