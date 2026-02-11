#pragma once

#include "timer.hpp"

namespace nano_hw::timer {

// Friend-Injection 用の Impl 関数宣言
void* AllocTimerInterfaceImpl(ICallbacks* callbacks, void* callback_context);
void FreeTimerInterfaceImpl(void* inst);
void ResetTimerImpl(void* inst);
void StartTimerImpl(void* inst);
void StopTimerImpl(void* inst);
std::chrono::milliseconds ReadTimerImpl(void* inst);

/// @brief Timer の実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl Timer の実装クラス (コールバックを持つ通常のクラス)
template <typename Impl>
class TimerImpl {
  friend void* AllocTimerInterfaceImpl(ICallbacks* callbacks,
                                       void* callback_context) {
    return new Impl(callbacks, callback_context);
  }

  friend void FreeTimerInterfaceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void ResetTimerImpl(void* inst) { static_cast<Impl*>(inst)->Reset(); }

  friend void StartTimerImpl(void* inst) { static_cast<Impl*>(inst)->Start(); }

  friend void StopTimerImpl(void* inst) { static_cast<Impl*>(inst)->Stop(); }

  friend std::chrono::milliseconds ReadTimerImpl(void* inst) {
    return static_cast<Impl*>(inst)->Read();
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocInterface(ICallbacks* callbacks, void* callback_context) {
  return AllocTimerInterfaceImpl(callbacks, callback_context);
}
void FreeInterface(void* inst) {
  FreeTimerInterfaceImpl(inst);
}
void ResetImpl(void* inst) {
  ResetTimerImpl(inst);
}
void StartImpl(void* inst) {
  StartTimerImpl(inst);
}
void StopImpl(void* inst) {
  StopTimerImpl(inst);
}
std::chrono::milliseconds ReadImpl(void* inst) {
  return ReadTimerImpl(inst);
}

}  // namespace nano_hw::timer
