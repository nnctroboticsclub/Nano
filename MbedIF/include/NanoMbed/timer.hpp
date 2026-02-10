#pragma once

#include <chrono>

#include <NanoHW/timer.hpp>
#include "common.hpp"

namespace {
namespace mbed {

// Forward declaration
class Timer;

// Per-instance data for each Timer
struct TimerInstanceData {
  void HandleTick() const;

 private:
  Callback<void()> tick_callback = nullptr;
  friend class Timer;
};

// MbedTimerConfig for callback routing
// Context is directly a pointer to Timer instance
struct MbedTimerConfig {
  struct OnTick {
    static void execute(void* context);
  };
};

class Timer {
 public:
  Timer() : dri_(this), data_(new TimerInstanceData()) {}

  virtual ~Timer() { delete data_; }

  void reset() { dri_.Reset(); }
  void start() { dri_.Start(); }
  void stop() { dri_.Stop(); }

  int read_ms() const {
    auto milliseconds = dri_.Read();
    return static_cast<int>(milliseconds.count());
  }

  std::chrono::milliseconds read() const { return dri_.Read(); }

  std::chrono::microseconds elapsed_time() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(dri_.Read());
  }

  // Attach callback for tick interrupt
  void attach(mbed::Callback<void()> func) { data_->tick_callback = func; }

  TimerInstanceData* GetInstanceData() { return data_; }

 private:
  nano_hw::timer::DynTimer<MbedTimerConfig> dri_;
  TimerInstanceData* data_;
};

// Implementation of TimerInstanceData methods
inline void TimerInstanceData::HandleTick() const {
  if (tick_callback != nullptr) {
    tick_callback();
  }
}

// Implementation of MbedTimerConfig::OnTick
inline void MbedTimerConfig::OnTick::execute(void* context) {
  auto* timer = static_cast<Timer*>(context);
  if (timer != nullptr) {
    timer->GetInstanceData()->HandleTick();
  }
}

}  // namespace mbed
}  // namespace