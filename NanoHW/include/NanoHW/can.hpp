#pragma once

#include <concepts>
#include <cstdint>

#include <NanoHW/pin.hpp>
#include <NanoHW/policies.hpp>
#include "pin.hpp"

namespace nano_hw::can {

struct CANMessage {
  uint32_t id = 0;
  uint8_t data[8] = {};  // NOLINT
  uint8_t len = 0;
};

struct CANFilter {
  enum class Type {
    kMask,
    kList,
  } filter_type;

  union {
    struct {
      uint32_t mask;
      uint32_t id;
    } mask_filter;
    struct {
      uint32_t id;
    } list_filter;
  } filter;
};

template <typename T>
concept CANConfig = Policy<typename T::OnCANReceived, void*, CANMessage> &&
                    Policy<typename T::OnCANTransmit, void*, CANMessage> &&
                    Policy<typename T::OnCANBusError, void*> &&
                    Policy<typename T::OnCANPassiveError, void*>;

struct DummyCANConfig {
  using OnCANReceived = nano_hw::Ignore;
  using OnCANTransmit = nano_hw::Ignore;
  using OnCANBusError = nano_hw::Ignore;
  using OnCANPassiveError = nano_hw::Ignore;
};
static_assert(CANConfig<DummyCANConfig>);

template <template <CANConfig> typename CanT>
concept CAN =
    requires(CanT<DummyCANConfig> value, Pin transmit_pin, Pin receive_pin,
             int frequency, CANMessage msg, int filter_num, CANFilter filter) {
  {CanT<DummyCANConfig>(transmit_pin, receive_pin, frequency)}
      ->std::same_as<CanT<DummyCANConfig>>;

  {value.SendMessage(msg)}->std::same_as<bool>;

  {value.TransmitErrors()}->std::same_as<int>;
  {value.ReceiveErrors()}->std::same_as<int>;

  {value.ResetPeripherals()}->std::same_as<void>;

  {value.SetFilter(filter_num, filter)}->std::same_as<void>;
  {value.DeactivateFilter(filter_num, filter)}->std::same_as<void>;
};

struct ICallbacks {
 public:
  virtual void OnCANReceived(void* context, CANMessage msg) = 0;
  virtual void OnCANTransmit(void* context, CANMessage msg) = 0;
  virtual void OnCANBusError(void* context) = 0;
  virtual void OnCANPassiveError(void* context) = 0;
};

void* AllocInterface(Pin transmit_pin, Pin receive_pin, int frequency,
                     ICallbacks* callbacks, void* callback_context);
void FreeInterface(void* interface);
bool SendMessageImpl(void* interface, CANMessage msg);
int TransmitErrorsImpl(void* interface);
int ReceiveErrorsImpl(void* interface);
void ResetPeripheralsImpl(void* interface);
void SetFilterImpl(void* interface, int filter_num, CANFilter filter);
void DeactivateFilterImpl(void* interface, int filter_num, CANFilter filter);
bool ReceiveRawImpl(void* interface, CANMessage& msg);

template <CANConfig Config>
class DynCAN {
  struct Callbacks : public ICallbacks {
   public:
    void OnCANReceived(void* context, CANMessage msg) final {
      Config::OnCANReceived::execute(context, msg);
    }
    void OnCANTransmit(void* context, CANMessage msg) final {
      Config::OnCANTransmit::execute(context, msg);
    }
    void OnCANBusError(void* context) final {
      Config::OnCANBusError::execute(context);
    }
    void OnCANPassiveError(void* context) final {
      Config::OnCANPassiveError::execute(context);
    }
  };

 public:
  explicit DynCAN(Pin transmit_pin, Pin receive_pin, int frequency)
      : interface_(AllocInterface(transmit_pin, receive_pin, frequency,
                                  &callbacks, this)) {}

  explicit DynCAN(Pin transmit_pin, Pin receive_pin, int frequency,
                  void* callback_context)
      : interface_(AllocInterface(transmit_pin, receive_pin, frequency,
                                  &callbacks, callback_context)) {}

  ~DynCAN() { FreeInterface(interface_); }

  bool SendMessage(CANMessage msg) { return SendMessageImpl(interface_, msg); }
  int TransmitErrors() { return TransmitErrorsImpl(interface_); }
  int ReceiveErrors() { return ReceiveErrorsImpl(interface_); }
  void ResetPeripherals() { ResetPeripheralsImpl(interface_); }
  void SetFilter(int filter_num, CANFilter filter) {
    SetFilterImpl(interface_, filter_num, filter);
  }
  void DeactivateFilter(int filter_num, CANFilter filter) {
    DeactivateFilterImpl(interface_, filter_num, filter);
  }

 private:
  static inline Callbacks callbacks = {};
  void* interface_;
};

static_assert(CAN<DynCAN>);

}  // namespace nano_hw::can
