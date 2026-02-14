#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

#include <NanoHW/pin.hpp>
#include <NanoHW/policies.hpp>
#include "pin.hpp"

namespace nano_hw::uart {

enum class Parity : uint8_t {
  kNone,
  kOdd,
  kEven,
  kForced1,
  kForced0,
};

template <typename T>
concept UARTConfig = Policy<typename T::OnUARTRx, void*, const uint8_t*, size_t> &&
                     Policy<typename T::OnUARTTx, void*, const uint8_t*, size_t>;

struct DummyUARTConfig {
  using OnUARTRx = nano_hw::Ignore;
  using OnUARTTx = nano_hw::Ignore;
};
static_assert(UARTConfig<DummyUARTConfig>);

template <template <UARTConfig> typename UartT>
concept UART =
    requires(UartT<DummyUARTConfig> value, Pin transmit_pin, Pin receive_pin,
             int frequency, void* buffer, size_t size) {
  {UartT<DummyUARTConfig>(transmit_pin, receive_pin, frequency)}
      ->std::same_as<UartT<DummyUARTConfig>>;
  {value.Rebaud(frequency)}->std::same_as<void>;
  {value.Send(buffer, size)}->std::same_as<size_t>;
  {value.Receive(buffer, size)}->std::same_as<size_t>;
};

struct ICallbacks {
 public:
  virtual void OnUARTRx(void* context, const uint8_t* buffer, size_t size) = 0;
  virtual void OnUARTTx(void* context, const uint8_t* buffer, size_t size) = 0;
};

void* AllocInterface(Pin transmit_pin, Pin receive_pin, int frequency,
                     ICallbacks* callbacks, void* callback_context);
void FreeInterface(void* interface);
void RebaudImpl(void* interface, int frequency);
size_t SendImpl(void* interface, void* buffer, size_t size);
size_t ReceiveImpl(void* interface, void* buffer, size_t size);

template <UARTConfig Config>
class DynUART {
  struct Callbacks : public ICallbacks {
   public:
    void OnUARTRx(void* context, const uint8_t* buffer, size_t size) final {
      Config::OnUARTRx::execute(context, buffer, size);
    }
    void OnUARTTx(void* context, const uint8_t* buffer, size_t size) final {
      Config::OnUARTTx::execute(context, buffer, size);
    }
  };

 public:
  explicit DynUART(Pin transmit_pin, Pin receive_pin, int frequency)
      : interface_(AllocInterface(transmit_pin, receive_pin, frequency,
                                  &callbacks, this)) {}

  explicit DynUART(Pin transmit_pin, Pin receive_pin, int frequency,
                   void* callback_context)
      : interface_(AllocInterface(transmit_pin, receive_pin, frequency,
                                  &callbacks, callback_context)) {}

  ~DynUART() { FreeInterface(interface_); }

  void Rebaud(int frequency) { RebaudImpl(interface_, frequency); }
  size_t Send(void* buffer, size_t size) {
    return SendImpl(interface_, buffer, size);
  }
  size_t Receive(void* buffer, size_t size) {
    return ReceiveImpl(interface_, buffer, size);
  }

 private:
  static inline Callbacks callbacks = {};
  void* interface_;
};

static_assert(UART<DynUART>);

}  // namespace nano_hw::uart
