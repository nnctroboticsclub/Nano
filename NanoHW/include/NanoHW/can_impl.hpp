#pragma once

#include "can.hpp"

namespace nano_hw::can {

// Friend-Injection 用の Impl 関数宣言
void* AllocCANInterfaceImpl(Pin transmit_pin, Pin receive_pin, int frequency,
                            ICallbacks* callbacks, void* callback_context);
void FreeCANInterfaceImpl(void* inst);
void ChangeBaudrateCANImpl(void* inst, int frequency);
void ChangeModeCANImpl(void* inst, CANMode mode);
bool SendMessageCANImpl(void* inst, CANMessage msg);
int TransmitErrorsCANImpl(void* inst);
int ReceiveErrorsCANImpl(void* inst);
void ResetPeripheralsCANImpl(void* inst);
void SetFilterCANImpl(void* inst, int filter_num, CANFilter filter);
void DeactivateFilterCANImpl(void* inst, int filter_num, CANFilter filter);
bool TryReceiveCANImpl(void* inst, CANMessage& msg);

/// @brief CAN conceptを満たす型から動的ディスパッチ関数を生成
/// @tparam CanT CAN conceptを満たすテンプレートクラス
template <template <CANConfig> typename CanT>
requires CAN<CanT> class CANImpl {
  // Config内のコールバックを呼び出すためのConfig
  struct CallbackConfig {
    struct OnCANReceived {
      static void execute(void* context, CANMessage msg) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnCANReceived(ctx->second, msg);
        }
      }
    };
    struct OnCANTransmit {
      static void execute(void* context, CANMessage msg) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnCANTransmit(ctx->second, msg);
        }
      }
    };
    struct OnCANBusError {
      static void execute(void* context) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnCANBusError(ctx->second);
        }
      }
    };
    struct OnCANPassiveError {
      static void execute(void* context) {
        auto* ctx = static_cast<std::pair<ICallbacks*, void*>*>(context);
        if (ctx->first != nullptr) {
          ctx->first->OnCANPassiveError(ctx->second);
        }
      }
    };
  };

  using ImplType = CanT<CallbackConfig>;

  friend void* AllocCANInterfaceImpl(Pin transmit_pin, Pin receive_pin,
                                     int frequency, ICallbacks* callbacks,
                                     void* callback_context) {
    auto* impl = new ImplType(transmit_pin, receive_pin, frequency);
    auto* ctx = new std::pair<ICallbacks*, void*>(callbacks, callback_context);
    return new std::pair<ImplType*, std::pair<ICallbacks*, void*>*>(impl, ctx);
  }

  friend void FreeCANInterfaceImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    delete pair->first;
    delete pair->second;
    delete pair;
  }

  friend void ChangeBaudrateCANImpl(void* inst, int frequency) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->ChangeBaudrate(frequency);
  }

  friend void ChangeModeCANImpl(void* inst, CANMode mode) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->ChangeMode(mode);
  }

  friend bool SendMessageCANImpl(void* inst, CANMessage msg) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    bool result = pair->first->SendMessage(msg);
    // コールバックを呼び出す
    if (result) {
      CallbackConfig::OnCANTransmit::execute(pair->second, msg);
    }
    return result;
  }

  friend int TransmitErrorsCANImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    return pair->first->TransmitErrors();
  }

  friend int ReceiveErrorsCANImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    return pair->first->ReceiveErrors();
  }

  friend void ResetPeripheralsCANImpl(void* inst) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->ResetPeripherals();
  }

  friend void SetFilterCANImpl(void* inst, int filter_num, CANFilter filter) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->SetFilter(filter_num, filter);
  }

  friend void DeactivateFilterCANImpl(void* inst, int filter_num,
                                      CANFilter filter) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    pair->first->DeactivateFilter(filter_num, filter);
  }

  friend bool TryReceiveCANImpl(void* inst, CANMessage& msg) {
    auto* pair =
        static_cast<std::pair<ImplType*, std::pair<ICallbacks*, void*>*>*>(
            inst);
    if constexpr (requires { pair->first->TryReceive(msg); }) {
      bool result = pair->first->TryReceive(msg);
      if (result) {
        // 受信成功時にコールバックを呼び出す
        CallbackConfig::OnCANReceived::execute(pair->second, msg);
      }
      return result;
    } else {
      // TryReceive が実装されていない場合は常に false
      return false;
    }
  }
};

// Friend-Injection で定義された関数はオブジェクトファイルに含まれないため、
// 通常の関数を挟んで実体を残す
void* AllocInterface(Pin transmit_pin, Pin receive_pin, int frequency,
                     ICallbacks* callbacks, void* callback_context) {
  return AllocCANInterfaceImpl(transmit_pin, receive_pin, frequency, callbacks,
                               callback_context);
}
void FreeInterface(void* inst) {
  FreeCANInterfaceImpl(inst);
}
void ChangeBaudrateImpl(void* inst, int frequency) {
  ChangeBaudrateCANImpl(inst, frequency);
}
void ChangeModeImpl(void* inst, CANMode mode) {
  ChangeModeCANImpl(inst, mode);
}
bool SendMessageImpl(void* inst, CANMessage msg) {
  return SendMessageCANImpl(inst, msg);
}
int TransmitErrorsImpl(void* inst) {
  return TransmitErrorsCANImpl(inst);
}
int ReceiveErrorsImpl(void* inst) {
  return ReceiveErrorsCANImpl(inst);
}
void ResetPeripheralsImpl(void* inst) {
  ResetPeripheralsCANImpl(inst);
}
void SetFilterImpl(void* inst, int filter_num, CANFilter filter) {
  SetFilterCANImpl(inst, filter_num, filter);
}
void DeactivateFilterImpl(void* inst, int filter_num, CANFilter filter) {
  DeactivateFilterCANImpl(inst, filter_num, filter);
}
bool TryReceiveImpl(void* inst, CANMessage& msg) {
  return TryReceiveCANImpl(inst, msg);
}

}  // namespace nano_hw::can
