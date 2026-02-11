#pragma once
#include <mbed.h>
#include <NanoHW/timer_impl.hpp>

namespace nano_mbed {
using nano_hw::timer::ICallbacks;

class MbedTimer {
 public:
  MbedTimer(ICallbacks* callbacks, void* callback_context) {
    (void)callbacks;         // Unused
    (void)callback_context;  // Unused
  }

  void Reset() { timer_.reset(); }

  void Start() { timer_.start(); }

  void Stop() { timer_.stop(); }

  std::chrono::milliseconds Read() {
    auto elapsed = timer_.elapsed_time();
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
  }

 private:
  mbed::Timer timer_;
  // ICallbacks* callbacks_;  // Unused - timer doesn't have callbacks yet
  // void* callback_context_;  // Unused
};
}  // namespace nano_mbed