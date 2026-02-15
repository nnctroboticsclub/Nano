#pragma once

#include <utility>

#include "can.hpp"
#include "policies.hpp"

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
  // Context: コールバック情報を一つの型に統合
  struct Context {
    ICallbacks* callbacks = nullptr;
    void* callback_context = nullptr;

    void OnCANReceived(CANMessage msg) {
      if (callbacks != nullptr) {
        callbacks->OnCANReceived(callback_context, msg);
      }
    }

    void OnCANTransmit(CANMessage msg) {
      if (callbacks != nullptr) {
        callbacks->OnCANTransmit(callback_context, msg);
      }
    }

    void OnCANBusError() {
      if (callbacks != nullptr) {
        callbacks->OnCANBusError(callback_context);
      }
    }

    void OnCANPassiveError() {
      if (callbacks != nullptr) {
        callbacks->OnCANPassiveError(callback_context);
      }
    }
  };

  // Config内のコールバックを呼び出すためのConfig
  struct CallbackConfig {
    using OnCANReceived = Direct<[](void* context, CANMessage msg) {
      auto* ctx = static_cast<Context*>(context);
      ctx->OnCANReceived(msg);
    }>;
    using OnCANTransmit = Direct<[](void* context, CANMessage msg) {
      auto* ctx = static_cast<Context*>(context);
      ctx->OnCANTransmit(msg);
    }>;
    using OnCANBusError = Direct<[](void* context) {
      auto* ctx = static_cast<Context*>(context);
      ctx->OnCANBusError();
    }>;
    using OnCANPassiveError = Direct<[](void* context) {
      auto* ctx = static_cast<Context*>(context);
      ctx->OnCANPassiveError();
    }>;
  };

  using ImplType = CanT<CallbackConfig>;

  // Instance: ImplType と Context を統合
  struct Instance {
    ImplType* impl = nullptr;
    Context* context = nullptr;
  };

  friend void* AllocCANInterfaceImpl(Pin transmit_pin, Pin receive_pin,
                                     int frequency, ICallbacks* callbacks,
                                     void* callback_context) {
    auto* ctx = new Context{callbacks, callback_context};
    auto* impl = new ImplType(transmit_pin, receive_pin, frequency, ctx);
    auto* instance = new Instance{impl, ctx};
    return instance;
  }

  friend void FreeCANInterfaceImpl(void* inst) {
    auto* instance = static_cast<Instance*>(inst);
    delete instance->impl;
    delete instance->context;
    delete instance;
  }

  friend void ChangeBaudrateCANImpl(void* inst, int frequency) {
    auto* instance = static_cast<Instance*>(inst);
    instance->impl->ChangeBaudrate(frequency);
  }

  friend void ChangeModeCANImpl(void* inst, CANMode mode) {
    auto* instance = static_cast<Instance*>(inst);
    instance->impl->ChangeMode(mode);
  }

  friend bool SendMessageCANImpl(void* inst, CANMessage msg) {
    auto* instance = static_cast<Instance*>(inst);
    bool result = instance->impl->SendMessage(msg);
    // コールバックを呼び出す
    if (result) {
      CallbackConfig::OnCANTransmit::execute(instance->context, msg);
    }
    return result;
  }

  friend int TransmitErrorsCANImpl(void* inst) {
    auto* instance = static_cast<Instance*>(inst);
    return instance->impl->TransmitErrors();
  }

  friend int ReceiveErrorsCANImpl(void* inst) {
    auto* instance = static_cast<Instance*>(inst);
    return instance->impl->ReceiveErrors();
  }

  friend void ResetPeripheralsCANImpl(void* inst) {
    auto* instance = static_cast<Instance*>(inst);
    instance->impl->ResetPeripherals();
  }

  friend void SetFilterCANImpl(void* inst, int filter_num, CANFilter filter) {
    auto* instance = static_cast<Instance*>(inst);
    instance->impl->SetFilter(filter_num, filter);
  }

  friend void DeactivateFilterCANImpl(void* inst, int filter_num,
                                      CANFilter filter) {
    auto* instance = static_cast<Instance*>(inst);
    instance->impl->DeactivateFilter(filter_num, filter);
  }

  friend bool TryReceiveCANImpl(void* inst, CANMessage& msg) {
    auto* instance = static_cast<Instance*>(inst);
    if constexpr (requires { instance->impl->TryReceive(msg); }) {
      bool result = instance->impl->TryReceive(msg);
      if (result) {
        // 受信成功時にコールバックを呼び出す
        CallbackConfig::OnCANReceived::execute(instance->context, msg);
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
