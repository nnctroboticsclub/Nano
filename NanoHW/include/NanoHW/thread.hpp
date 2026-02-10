#pragma once

#include <concepts>
#include <cstdint>
#include <functional>
#include <string>

enum ThreadPriority {
  ThreadPriorityIdle = -3,         ///< Priority: idle (lowest)
  ThreadPriorityLow = -2,          ///< Priority: low
  ThreadPriorityBelowNormal = -1,  ///< Priority: below normal
  ThreadPriorityNormal = 0,        ///< Priority: normal (default)
  ThreadPriorityAboveNormal = +1,  ///< Priority: above normal
  ThreadPriorityHigh = +2,         ///< Priority: high
  ThreadPriorityRealtime = +3,     ///< Priority: realtime (highest)
};

namespace nano_hw::thread {

template <typename T>
concept Thread = requires(T value, ThreadPriority priority, uint32_t stack_size,
                          unsigned char* stack_mem, const char* name,
                          std::function<void()> task) {
  {T(priority, stack_size, stack_mem, name)}->std::same_as<T>;
  {value.Start(task)}->std::same_as<void>;
  {value.Join()}->std::same_as<void>;
  {value.Terminate()}->std::same_as<void>;
  {value.SetPriority(priority)}->std::same_as<void>;
  {value.GetPriority()}->std::same_as<ThreadPriority>;
};

void* AllocInterface(ThreadPriority priority, uint32_t stack_size,
                     unsigned char* stack_mem, const char* name);
void FreeInterface(void* interface);
void StartImpl(void* interface, std::function<void()> task);
void JoinImpl(void* interface);
void TerminateImpl(void* interface);
void SetPriorityImpl(void* interface, ThreadPriority priority);
ThreadPriority GetPriorityImpl(void* interface);

class DynThread {
 public:
  explicit DynThread(ThreadPriority priority = ThreadPriorityNormal,
                     uint32_t stack_size = 4096,
                     unsigned char* stack_mem = nullptr,
                     const char* name = nullptr)
      : interface_(AllocInterface(priority, stack_size, stack_mem, name)) {}

  ~DynThread() { FreeInterface(interface_); }

  void Start(std::function<void()> task) { StartImpl(interface_, task); }
  void Join() { JoinImpl(interface_); }
  void Terminate() { TerminateImpl(interface_); }
  void SetPriority(ThreadPriority priority) {
    SetPriorityImpl(interface_, priority);
  }
  ThreadPriority GetPriority() { return GetPriorityImpl(interface_); }

 private:
  void* interface_;
};

static_assert(Thread<DynThread>);

}  // namespace nano_hw::thread
