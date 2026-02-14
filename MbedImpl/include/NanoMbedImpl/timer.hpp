#pragma once
#include <mbed.h>
#include <NanoHW/timer.hpp>

namespace nano_mbed {

// MbedTimer implementation satisfies Timer concept
// It is a template that accepts TimerConfig and implements the Timer interface
template <nano_hw::timer::TimerConfig Config>
class MbedTimer {
 public:
  // Default constructor for Timer concept satisfaction
  MbedTimer() = default;

  MbedTimer(nano_hw::timer::ICallbacks* callbacks, void* callback_context) {
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
};

// Verify MbedTimer satisfies Timer concept
static_assert(nano_hw::timer::Timer<MbedTimer>);

}  // namespace nano_mbed