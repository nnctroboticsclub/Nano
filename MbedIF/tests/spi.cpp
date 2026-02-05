#include <gtest/gtest.h>

#include <mbed.h>

TEST(SPITest, WriteSingleByte) {
  SPI spi(NC, NC, NC);

  const int value = 0x5A;
  const int result = spi.write(value);

  EXPECT_EQ(result, value);
}

TEST(SPITest, WriteBuffer) {
  SPI spi(NC, NC, NC);

  const char tx[] = {0x01, 0x02, 0x03, 0x04};
  char rx[4] = {};

  const int transferred = spi.write(tx, 4, rx, 4);

  EXPECT_EQ(transferred, 4);
  EXPECT_EQ(rx[0], tx[0]);
  EXPECT_EQ(rx[1], tx[1]);
  EXPECT_EQ(rx[2], tx[2]);
  EXPECT_EQ(rx[3], tx[3]);
}

TEST(SPITest, ConfigureFormatAndFrequency) {
  SPI spi(NC, NC, NC);

  spi.frequency(2000000);
  spi.format(8, 0);
  spi.format(8, 1);
  spi.format(8, 2);
  spi.format(8, 3);

  SUCCEED();
}
