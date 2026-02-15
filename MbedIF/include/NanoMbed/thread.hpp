#pragma once

#include <cstdint>
#include <functional>

#include <NanoHW/thread.hpp>

#include "common.hpp"

// Legacy compatibility aliases
constexpr ThreadPriority osPriorityIdle = ::ThreadPriorityIdle;
constexpr ThreadPriority osPriorityLow = ::ThreadPriorityLow;
constexpr ThreadPriority osPriorityBelowNormal = ::ThreadPriorityBelowNormal;
constexpr ThreadPriority osPriorityNormal = ::ThreadPriorityNormal;
constexpr ThreadPriority osPriorityAboveNormal = ::ThreadPriorityAboveNormal;
constexpr ThreadPriority osPriorityHigh = ::ThreadPriorityHigh;
constexpr ThreadPriority osPriorityRealtime = ::ThreadPriorityRealtime;

namespace rtos {
class Thread {
 public:
  Thread(::ThreadPriority priority = ::ThreadPriorityNormal,
         uint32_t stack_size = 4096, unsigned char* stack_mem = nullptr,
         const char* name = nullptr)
      : dri_(priority, stack_size, stack_mem, name) {}

  ~Thread() = default;

  void start(mbed::Callback<void()> task) { dri_.Start(task); }

  void join() { dri_.Join(); }

  void terminate() { dri_.Terminate(); }

  void set_priority(::ThreadPriority priority) { dri_.SetPriority(priority); }

  ::ThreadPriority get_priority() { return dri_.GetPriority(); }

 private:
  nano_hw::thread::DynThread dri_;
};

}  // namespace rtos
