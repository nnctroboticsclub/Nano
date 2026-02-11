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
bool ReceiveRawCANImpl(void* inst, CANMessage& msg);

/// @brief CAN の実装クラスから動的ディスパッチ関数を生成
/// @tparam Impl CAN の実装クラス (コールバックを持つ通常のクラス)
template <typename Impl>
class CANImpl {
  friend void* AllocCANInterfaceImpl(Pin transmit_pin, Pin receive_pin,
                                     int frequency, ICallbacks* callbacks,
                                     void* callback_context) {
    return new Impl(transmit_pin, receive_pin, frequency, callbacks,
                    callback_context);
  }

  friend void FreeCANInterfaceImpl(void* inst) {
    delete static_cast<Impl*>(inst);
  }

  friend void ChangeBaudrateCANImpl(void* inst, int frequency) {
    static_cast<Impl*>(inst)->ChangeBaudrate(frequency);
  }

  friend void ChangeModeCANImpl(void* inst, CANMode mode) {
    static_cast<Impl*>(inst)->ChangeMode(mode);
  }

  friend bool SendMessageCANImpl(void* inst, CANMessage msg) {
    return static_cast<Impl*>(inst)->SendMessage(msg);
  }

  friend int TransmitErrorsCANImpl(void* inst) {
    return static_cast<Impl*>(inst)->TransmitErrors();
  }

  friend int ReceiveErrorsCANImpl(void* inst) {
    return static_cast<Impl*>(inst)->ReceiveErrors();
  }

  friend void ResetPeripheralsCANImpl(void* inst) {
    static_cast<Impl*>(inst)->ResetPeripherals();
  }

  friend void SetFilterCANImpl(void* inst, int filter_num, CANFilter filter) {
    static_cast<Impl*>(inst)->SetFilter(filter_num, filter);
  }

  friend void DeactivateFilterCANImpl(void* inst, int filter_num,
                                      CANFilter filter) {
    static_cast<Impl*>(inst)->DeactivateFilter(filter_num, filter);
  }

  friend bool ReceiveRawCANImpl(void* inst, CANMessage& msg) {
    return static_cast<Impl*>(inst)->ReceiveRaw(msg);
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
bool ReceiveRawImpl(void* inst, CANMessage& msg) {
  return ReceiveRawCANImpl(inst, msg);
}

}  // namespace nano_hw::can
