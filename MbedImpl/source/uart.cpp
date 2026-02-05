#include <mbed.h>
#include <NanoHW/uart.hpp>

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

void* nano_hw::uart::AllocInterface(nano_hw::Pin transmit_pin,
                                    nano_hw::Pin receive_pin, int frequency,
                                    ICallbacks* callbacks,
                                    void* callback_context) {
  return new MbedUART(transmit_pin, receive_pin, frequency, callbacks,
                      callback_context);
}

void nano_hw::uart::FreeInterface(void* interface) {
  delete static_cast<MbedUART*>(interface);
}

void nano_hw::uart::RebaudImpl(void* interface, int frequency) {
  static_cast<MbedUART*>(interface)->Rebaud(frequency);
}

size_t nano_hw::uart::SendImpl(void* interface, void* buffer, size_t size) {
  return static_cast<MbedUART*>(interface)->Send(buffer, size);
}

size_t nano_hw::uart::ReceiveImpl(void* interface, void* buffer, size_t size) {
  return static_cast<MbedUART*>(interface)->Receive(buffer, size);
}
