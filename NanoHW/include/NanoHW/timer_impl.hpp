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

/// @brief Timer conceptを満たす型から動的ディスパッチ関数を生成
/// @tparam TimerT Timer conceptを満たすテンプレートクラス
template <template <TimerConfig> typename TimerT>
requires Timer<TimerT> class TimerImpl {
  // Config内のコールバックを呼び出すためのConfig
  struct CallbackConfig {
    struct OnTick {
      static void execute(void* context) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnTick(ctx->second);
        }
      }
    };
  };

  using ImplType = TimerT<CallbackConfig>;

  friend void* AllocTimerInterfaceImpl(ICallbacks* callbacks,
                                       void* callback_context) {
    auto* impl = new ImplType();
    auto* ctx = new std::pair<ICallbacks*, void*>(callbacks, callback_context);
    return new std::pair<ImplType*, std::pair<ICallbacks*, void*>*>(impl, ctx);
  }

  friend void FreeTimerInterfaceImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    delete pair->first;
    delete pair->second;
    delete pair;
  }

  friend void ResetTimerImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->Reset();
  }

  friend void StartTimerImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->Start();
  }

  friend void StopTimerImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->Stop();
  }

  friend std::chrono::milliseconds ReadTimerImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    return pair->first->Read();
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
