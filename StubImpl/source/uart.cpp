#include <NanoHW/pin.hpp>
#include <NanoHW/uart_impl.hpp>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>

using nano_hw::uart::ICallbacks;

class MockUART {
 public:
  MockUART(nano_hw::Pin tx, nano_hw::Pin rx, int baud_rate,
           ICallbacks* callbacks, void* callback_context)
      : tx_(tx),
        rx_(rx),
        baud_rate_(baud_rate),
        callbacks_(callbacks),
        callback_context_(callback_context) {
    std::cout << "MockUART initialized: TX " << tx_.number << ", RX "
              << rx_.number << ", baud " << baud_rate_ << "\n";
  }

  void Rebaud(int baud_rate) {
    baud_rate_ = baud_rate;
    std::cout << "MockUART Rebaud: " << baud_rate_ << "\n";
  }

  size_t Send(void* buffer, size_t size) {
    std::cout << "MockUART Send: size " << size << "\n";
    if (callbacks_ != nullptr) {
      callbacks_->OnUARTTx(callback_context_, buffer, size);
    }
    return size;
  }

  size_t Receive(void* buffer, size_t size) {
    std::cout << "MockUART Receive: size " << size << "\n";
    if (buffer != nullptr && size > 0) {
      std::memset(buffer, 0xA5, size);
    }
    if (callbacks_ != nullptr) {
      callbacks_->OnUARTRx(callback_context_, buffer, size);
    }
    return size;
  }

 private:
  nano_hw::Pin tx_;
  nano_hw::Pin rx_;
  int baud_rate_;
  ICallbacks* callbacks_;
  void* callback_context_;
};

// UARTImpl をインスタンス化して Friend-Injection を有効化
template class nano_hw::uart::UARTImpl<MockUART>;
