#pragma once

#include <chrono>
#include <cstdio>
#include <cstring>

#include <NanoMbed/digital_out.hpp>

namespace mbed {

// Common
template <typename T, size_t N>
class CircularBuffer {
 public:
  void push(T);
  bool empty();
  void pop(T&);
};

template <typename T>
class Callback {};

template <typename R, typename... Args>
class Callback<R(Args...)> {
 public:
  template <typename U, typename V>
  Callback(U* obj, R (V::*method)(Args...)) {}
};

enum EnableState : uint8_t {
  DISABLE,
  ENABLE,
};

// HAL
enum CANFormat : uint8_t { CANStandard, CANExtended, CANAny };
enum CANFilterMode_t : uint8_t {
  CAN_FILTERMODE_IDMASK = 0,
  CAN_FILTERMODE_IDLIST = 1,
};
enum CANFilterScale_t : uint8_t {
  CAN_FILTERSCALE_16BIT = 0,
  CAN_FILTERSCALE_32BIT = 1,
};
enum CANFilterFIFO_t : uint8_t {
  CAN_FILTER_FIFO0 = 0,
  CAN_FILTER_FIFO1 = 1,
};
struct CAN_FilterConfTypeDef {
  int FilterNumber;
  CANFilterMode_t FilterMode;
  CANFilterScale_t FilterScale;
  uint32_t FilterIdHigh;
  uint32_t FilterIdLow;
  uint32_t FilterMaskIdHigh;
  uint32_t FilterMaskIdLow;

  CANFilterFIFO_t FilterFIFOAssignment;
  EnableState FilterActivation;
  uint8_t BankNumber;
};
struct CANMessage {
  uint32_t id = 0;
  uint8_t data[8]{};
  uint8_t len = 0;

  CANMessage() = default;

  CANMessage(uint32_t id, uint8_t* data, uint8_t len) : id(id), len(len) {
    std::memcpy(this->data, data, len);
  }
};

enum HAL_StatusTypeDef {
  HAL_OK = 0,
};
struct HAL_CAN_TypeDef {};
HAL_StatusTypeDef HAL_CAN_ConfigFilter(HAL_CAN_TypeDef* hcan,
                                       CAN_FilterConfTypeDef* sFilterConfig);
// LL API
struct can_t {
  HAL_CAN_TypeDef CanHandle;
};

// MBED
class CAN {
 public:
  enum IrqType : uint8_t {
    RxIrq,
    TxIrq,
    BeIrq,
    EpIrq,
  };

  CAN(PinName receive_pin, PinName send_pin);
  CAN(PinName receive_pin, PinName send_pin, int frequency);
  virtual ~CAN();

  virtual void lock();
  virtual void unlock();

  int rderror();
  int tderror();

  int write(const CANMessage& msg);
  int read(CANMessage& msg, int handle = 0);
  void reset();
  void attach(Callback<void()> func, IrqType irq_type);

 protected:
  can_t _can;
};

// Timer
class Timer {
 public:
  void reset();
  void start();

  int read_ms();
};

// RTOS
namespace ThisThread {
void sleep_for(std::chrono::milliseconds ms);
}  // namespace ThisThread
}  // namespace mbed

using namespace mbed;
using namespace std;