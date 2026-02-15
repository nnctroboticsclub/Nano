#pragma once

#include <concepts>
#include <cstdint>

#include "pin.hpp"
#include "policies.hpp"

namespace nano_hw::can {
enum class CANMessageFormat { kStandard, kExtended };

struct CANMessage {
  uint32_t id = 0;
  uint8_t data[8] = {};  // NOLINT
  uint8_t len = 0;
  CANMessageFormat format;
};

enum class CANMode { kNormal, kLoopback };

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
concept CAN = requires(CanT<DummyCANConfig> value, Pin transmit_pin,
                       Pin receive_pin, int frequency, void* instance,
                       CANMessage msg, int filter_num, CANFilter filter) {
  {CanT<DummyCANConfig>(transmit_pin, receive_pin, frequency)}
      ->std::same_as<CanT<DummyCANConfig>>;
  {CanT<DummyCANConfig>(transmit_pin, receive_pin, frequency, instance)}
      ->std::same_as<CanT<DummyCANConfig>>;

  {value.SendMessage(msg)}->std::same_as<bool>;

  {value.TransmitErrors()}->std::same_as<int>;
  {value.ReceiveErrors()}->std::same_as<int>;

  {value.ResetPeripherals()}->std::same_as<void>;
  {value.ChangeMode(CANMode::kNormal)}->std::same_as<void>;
  {value.ChangeBaudrate(frequency)}->std::same_as<void>;

  {value.SetFilter(filter_num, filter)}->std::same_as<void>;
  {value.DeactivateFilter(filter_num, filter)}->std::same_as<void>;
};

/// @brief CAN with polling receive capability (ISR-safe, mutex-free)
template <template <CANConfig> typename CanT>
concept CANWithPolling =
    CAN<CanT> && requires(CanT<DummyCANConfig> value, CANMessage& msg) {
  {value.TryReceive(msg)}->std::same_as<bool>;
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
void ChangeBaudrateImpl(void* interface, int frequency);
void ChangeModeImpl(void* interface, CANMode mode);
bool SendMessageImpl(void* interface, CANMessage msg);
int TransmitErrorsImpl(void* interface);
int ReceiveErrorsImpl(void* interface);
void ResetPeripheralsImpl(void* interface);
void SetFilterImpl(void* interface, int filter_num, CANFilter filter);
void DeactivateFilterImpl(void* interface, int filter_num, CANFilter filter);
bool TryReceiveImpl(void* interface, CANMessage& msg);

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
  void ChangeMode(CANMode mode) { ChangeModeImpl(interface_, mode); }
  void ChangeBaudrate(int frequency) {
    ChangeBaudrateImpl(interface_, frequency);
  }

  void SetFilter(int filter_num, CANFilter filter) {
    SetFilterImpl(interface_, filter_num, filter);
  }
  void DeactivateFilter(int filter_num, CANFilter filter) {
    DeactivateFilterImpl(interface_, filter_num, filter);
  }

  bool TryReceive(CANMessage& msg) { return TryReceiveImpl(interface_, msg); }

 private:
  static inline Callbacks callbacks = {};
  void* interface_;
};

static_assert(CAN<DynCAN>);
static_assert(CANWithPolling<DynCAN>);

}  // namespace nano_hw::can
