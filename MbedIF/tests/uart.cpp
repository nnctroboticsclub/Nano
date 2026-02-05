#include <gtest/gtest.h>

#include <mbed.h>

using mbed::UnbufferedSerial;

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
