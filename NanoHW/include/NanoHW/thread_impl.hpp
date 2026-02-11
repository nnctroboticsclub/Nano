#pragma once

#include "thread.hpp"

namespace nano_hw::thread {

// Friend-Injection 用の Impl 関数宣言
void* AllocThreadInterfaceImpl(ThreadPriority priority, uint32_t stack_size,
                               unsigned char* stack_mem, const char* name);
void FreeThreadInterfaceImpl(void* inst);
void StartThreadImpl(void* inst, std::function<void()> task);
void JoinThreadImpl(void* inst);
void TerminateThreadImpl(void* inst);
void SetPriorityThreadImpl(void* inst, ThreadPriority priority);
ThreadPriority GetPriorityThreadImpl(void* inst);

/// @brief Thread concept を満たす実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl Thread concept を満たす実装クラス
template <Thread Impl>
class ThreadImpl {
  friend void* AllocThreadInterfaceImpl(ThreadPriority priority,
                                        uint32_t stack_size,
                                        unsigned char* stack_mem,
                                        const char* name) {
    return new Impl(priority, stack_size, stack_mem, name);
  }

  friend void FreeThreadInterfaceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void StartThreadImpl(void* inst, std::function<void()> task) {
    static_cast<Impl*>(inst)->Start(task);
  }

  friend void JoinThreadImpl(void* inst) { static_cast<Impl*>(inst)->Join(); }

  friend void TerminateThreadImpl(void* inst) {
    static_cast<Impl*>(inst)->Terminate();
  }

  friend void SetPriorityThreadImpl(void* inst, ThreadPriority priority) {
    static_cast<Impl*>(inst)->SetPriority(priority);
  }

  friend ThreadPriority GetPriorityThreadImpl(void* inst) {
    return static_cast<Impl*>(inst)->GetPriority();
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocInterface(ThreadPriority priority, uint32_t stack_size,
                     unsigned char* stack_mem, const char* name) {
  return AllocThreadInterfaceImpl(priority, stack_size, stack_mem, name);
}
void FreeInterface(void* inst) {
  FreeThreadInterfaceImpl(inst);
}
void StartImpl(void* inst, std::function<void()> task) {
  StartThreadImpl(inst, task);
}
void JoinImpl(void* inst) {
  JoinThreadImpl(inst);
}
void TerminateImpl(void* inst) {
  TerminateThreadImpl(inst);
}
void SetPriorityImpl(void* inst, ThreadPriority priority) {
  SetPriorityThreadImpl(inst, priority);
}
ThreadPriority GetPriorityImpl(void* inst) {
  return GetPriorityThreadImpl(inst);
}

}  // namespace nano_hw::thread
