#include <mbed.h>
#include <NanoHW/timer.hpp>

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

void* nano_hw::timer::AllocInterface(ICallbacks* callbacks,
                                     void* callback_context) {
  return new MbedTimer(callbacks, callback_context);
}

void nano_hw::timer::FreeInterface(void* interface) {
  delete static_cast<MbedTimer*>(interface);
}

void nano_hw::timer::ResetImpl(void* interface) {
  static_cast<MbedTimer*>(interface)->Reset();
}

void nano_hw::timer::StartImpl(void* interface) {
  static_cast<MbedTimer*>(interface)->Start();
}

void nano_hw::timer::StopImpl(void* interface) {
  static_cast<MbedTimer*>(interface)->Stop();
}

std::chrono::milliseconds nano_hw::timer::ReadImpl(void* interface) {
  return static_cast<MbedTimer*>(interface)->Read();
}
