#pragma once
#include <mbed.h>
#include <NanoHW/timer.hpp>

namespace nano_mbed {

template <nano_hw::timer::TimerConfig Config>
class MbedTimer {
 public:
  MbedTimer() = default;

  MbedTimer(nano_hw::timer::ICallbacks* callbacks, void* callback_context)
      : callbacks_(callbacks), callback_context_(callback_context) {}

  void Reset() {
    if (std::holds_alternative<mbed::Timer>(instance_)) {
      std::get<mbed::Timer>(instance_).reset();
    }
  }
  void Start() {
    if (std::holds_alternative<mbed::Timer>(instance_)) {
      std::get<mbed::Timer>(instance_).start();
    }
  }
  void Stop() {
    if (std::holds_alternative<mbed::Timer>(instance_)) {
      std::get<mbed::Timer>(instance_).stop();
    }
  }

  std::chrono::milliseconds Read() {
    if (!std::holds_alternative<mbed::Timer>(instance_)) {
      return std::chrono::milliseconds(0);
    }
    auto& timer = std::get<mbed::Timer>(instance_);
    auto elapsed = timer.elapsed_time();
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
  }

  bool EnableTick(std::chrono::milliseconds interval) {
    if (!std::holds_alternative<mbed::Ticker>(instance_)) {
      instance_.emplace<mbed::Ticker>();
    }
    auto& ticker = std::get<mbed::Ticker>(instance_);
    ticker.attach(
        [this]() {
          if (callbacks_ != nullptr) {
            callbacks_->OnTick(callback_context_);
          }
        },
        interval);
    return true;
  }

 private:
  std::variant<mbed::Timer, mbed::Ticker> instance_ = mbed::Timer{};

  nano_hw::timer::ICallbacks* callbacks_ = nullptr;
  void* callback_context_ = nullptr;
};

// Verify MbedTimer satisfies Timer concept
static_assert(nano_hw::timer::Timer<MbedTimer>);

}  // namespace nano_mbed