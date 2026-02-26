#include "uart.hpp"
#include <gtest/gtest.h>

#include <mbed.h>
#include "NanoHW/uart_impl.hpp"

using mbed::UnbufferedSerial;

template struct nano_hw::uart::UARTImpl<nano_stub::MockUART>;

TEST(UARTTest, WriteBuffer) {
  UnbufferedSerial uart(NC, NC, 115200);

  const char tx[] = "hello";
  const size_t written = uart.write(tx, sizeof(tx));

  EXPECT_EQ(written, sizeof(tx));
}

TEST(UARTTest, ReadBuffer) {
  UnbufferedSerial uart(NC, NC, 115200);

  char rx[4] = {};
  const size_t read = uart.read(rx, sizeof(rx));

  EXPECT_EQ(read, sizeof(rx));
  EXPECT_EQ(static_cast<unsigned char>(rx[0]), 0xA5);
  EXPECT_EQ(static_cast<unsigned char>(rx[1]), 0xA5);
  EXPECT_EQ(static_cast<unsigned char>(rx[2]), 0xA5);
  EXPECT_EQ(static_cast<unsigned char>(rx[3]), 0xA5);
}

TEST(UARTTest, BaudChange) {
  UnbufferedSerial uart(NC, NC, 9600);
  uart.baud(115200);

  SUCCEED();
}

TEST(UARTTest, Format) {
  UnbufferedSerial uart(NC, NC, 9600);
  uart.format(8, SerialBase::None, 1);
  uart.format(8, SerialBase::None, 2);
  uart.format(8, SerialBase::Odd, 1);
  uart.format(8, SerialBase::Odd, 2);
  uart.format(8, SerialBase::Even, 1);
  uart.format(8, SerialBase::Even, 2);

  SUCCEED();
}

TEST(UARTTest, RxIrq) {
  struct Handler {
    void Rx() {
      char buf[2];
      size_t len = serial_.read(buf, 2);

      ASSERT_EQ(len, 2);
      EXPECT_EQ(static_cast<unsigned char>(buf[0]), 0xA5);
      EXPECT_EQ(static_cast<unsigned char>(buf[1]), 0x5A);
    }

    UnbufferedSerial serial_;
  };

  UnbufferedSerial uart(NC, NC, 9600);
  auto handler = new Handler{uart};
  uart.attach(Callback<void()>(handler, &Handler::Rx), UnbufferedSerial::RxIrq);

  SUCCEED();
}
