#include "NanoHW/timer.hpp"

#include <chrono>
#include <iostream>

using nano_hw::timer::ICallbacks;

class MockTimer {
 public:
  MockTimer(ICallbacks* callbacks, void* callback_context)
      : callbacks_(callbacks),
        callback_context_(callback_context),
        start_time_(std::chrono::steady_clock::now()),
        accumulated_time_(0) {
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

  // Simulate tick interrupt (for testing purposes)
  void SimulateTick() {
    if (callbacks_ != nullptr) {
      callbacks_->OnTick(callback_context_);
    }
  }

 private:
  ICallbacks* callbacks_;
  void* callback_context_;
  std::chrono::steady_clock::time_point start_time_;
  bool is_running_ = false;
  std::chrono::milliseconds accumulated_time_;
};

void* nano_hw::timer::AllocInterface(ICallbacks* callbacks,
                                     void* callback_context) {
  return new MockTimer(callbacks, callback_context);
}

void nano_hw::timer::FreeInterface(void* interface) {
  delete static_cast<MockTimer*>(interface);
}

void nano_hw::timer::ResetImpl(void* interface) {
  static_cast<MockTimer*>(interface)->Reset();
}

void nano_hw::timer::StartImpl(void* interface) {
  static_cast<MockTimer*>(interface)->Start();
}

void nano_hw::timer::StopImpl(void* interface) {
  static_cast<MockTimer*>(interface)->Stop();
}

std::chrono::milliseconds nano_hw::timer::ReadImpl(void* interface) {
  return static_cast<MockTimer*>(interface)->Read();
}
