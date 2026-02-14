#pragma once

#include <mbed.h>
#include <Nano/no_mutex_lifo.hpp>
#include <NanoHW/uart.hpp>
#include <cstddef>

#include "./thread.hpp"

namespace nano_mbed {
template <nano_hw::uart::UARTConfig UARTConfig>
class MbedUART {
  static constexpr size_t kStackSize = 8192;

  void Init(int baudrate) {
    if (serial_ != nullptr) {
      return;
    }

    serial_ = new mbed::UnbufferedSerial(tx, rx, baudrate);
    serial_->enable_input(true);
    serial_->enable_output(true);
  }

  void Deinit() {
    if (serial_ == nullptr) {
      return;
    }
    serial_->enable_input(false);
    serial_->enable_output(false);
    serial_->close();
    delete serial_;
    serial_ = nullptr;
  }

 public:
  MbedUART(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency)
      : MbedUART(transmit_pin, receive_pin, frequency, this) {}

  MbedUART(nano_hw::Pin transmit_pin, nano_hw::Pin receive_pin, int frequency,
           void* cb_ctx)
      : serial_(nullptr),
        tx(static_cast<PinName>(transmit_pin.number)),
        rx(static_cast<PinName>(receive_pin.number)),
        cb_ctx_(cb_ctx),
        thread_dispatch(ThreadPriorityNormal, kStackSize, nullptr,
                        "UARTStream-Dispatch") {
    Init(frequency);

    serial_->attach([this]() {
      if (serial_->readable()) {
        static uint8_t buf[128] = {};
        const auto len = serial_->read(buf, 128);

        buffer.PushN(buf, len);
      }
    });

    thread_dispatch.Start([this]() {
      using namespace std::chrono_literals;

      this->is_running = true;
      while (!serial_) {
        ThisThread::sleep_for(10ms);
      }

      while (!stop_token) {
        if (not buffer.Empty()) {
          static uint8_t buf[128] = {};
          auto len = buffer.Size() < 128 ? buffer.Size() : 128;
          buffer.PopNTo(len, buf);
          UARTConfig::OnUARTRx::execute(cb_ctx_, buf, len);
        }

        ThisThread::sleep_for(100ms);
      }

      this->is_running = false;
    });

    while (!is_running) {
      ThisThread::sleep_for(10ms);
    }
  }

  ~MbedUART() {
    using namespace std::chrono_literals;

    stop_token = true;
    while (is_running) {
      ThisThread::sleep_for(10ms);
    }
    Deinit();
  }

  void Rebaud(int baudrate) {
    Deinit();
    Init(baudrate);
  }

  size_t Send(void* buffer, size_t size) {
    if (serial_ == nullptr) {
      return 0;
    }

    const auto written = serial_->write(buffer, size);
    if (written > 0) {
      UARTConfig::OnUARTTx::execute(cb_ctx_,
                                    static_cast<const uint8_t*>(buffer),
                                    static_cast<size_t>(written));
    }
    return written > 0 ? static_cast<size_t>(written) : 0U;
  }

  size_t Receive(void* buffer, size_t size) {
    if (serial_ == nullptr) {
      return 0;
    }
    const auto read = serial_->read(buffer, size);
    if (read > 0) {
      UARTConfig::OnUARTRx(cb_ctx_, buffer, static_cast<size_t>(read));
    }
    return read > 0 ? static_cast<size_t>(read) : 0U;
  }

 private:
  mbed::UnbufferedSerial* serial_;
  PinName tx, rx;
  void* cb_ctx_;

  Nano::collection::NoMutexLIFO<uint8_t, 32> buffer;
  MbedThread thread_dispatch;
  bool is_running = false;
  bool stop_token = false;
};

static_assert(nano_hw::uart::UART<MbedUART>);
}  // namespace nano_mbed