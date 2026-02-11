#include <mbed.h>
#include <NanoHW/uart_impl.hpp>

#include <cstddef>

using nano_hw::uart::ICallbacks;

class MbedUART {
 public:
  MbedUART(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency,
           ICallbacks* callbacks, void* callback_context)
      : serial_(static_cast<PinName>(transmit_pin.number),
                static_cast<PinName>(receive_pin.number), frequency),
        callbacks_(callbacks),
        callback_context_(callback_context) {}

  void Rebaud(int frequency) { serial_.baud(frequency); }

  size_t Send(void* buffer, size_t size) {
    const auto written = serial_.write(buffer, size);
    if (callbacks_ != nullptr && written > 0) {
      callbacks_->OnUARTTx(callback_context_, buffer,
                           static_cast<size_t>(written));
    }
    return written > 0 ? static_cast<size_t>(written) : 0U;
  }

  size_t Receive(void* buffer, size_t size) {
    const auto read = serial_.read(buffer, size);
    if (callbacks_ != nullptr && read > 0) {
      callbacks_->OnUARTRx(callback_context_, buffer,
                           static_cast<size_t>(read));
    }
    return read > 0 ? static_cast<size_t>(read) : 0U;
  }

 private:
  mbed::UnbufferedSerial serial_;
  ICallbacks* callbacks_;
  void* callback_context_;
};

// UARTImpl をインスタンス化して Friend-Injection を有効化
template class nano_hw::uart::UARTImpl<MbedUART>;
