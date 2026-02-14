#include <mbed.h>
#include <NanoHW/spi.hpp>
#include <NanoMbedImpl/spi.hpp>
#include <vector>

using namespace nano_hw;
using namespace nano_hw::spi;

// Test configuration
struct TestConfig {
  struct OnTransfer {
    static void execute(void* ctx, const std::vector<uint8_t>& tx_data,
                        const std::vector<uint8_t>& rx_data) {
      (void)ctx;
      (void)tx_data;
      printf("SPI Transfer complete: %zu bytes\n", rx_data.size());
      for (size_t i = 0; i < rx_data.size() && i < 8; ++i) {
        printf("  rx[%zu] = 0x%02x\n", i, rx_data[i]);
      }
    }
  };
};

int main() {
  printf("\n=== MbedImpl SPI Test ===\n");

  // SPI pins for NUCLEO_F446RE
  // SPI1: MOSI=PA7, MISO=PA6, SCK=PA5, CS=PA4
  Pin mosi = {PA_7};
  Pin miso = {PA_6};
  Pin sck = {PA_5};
  // CS is managed manually if needed

  printf("Creating MbedSPI instance...\n");
  nano_mbed::MbedSPI<TestConfig> spi(miso, mosi, sck, 1000000);

  printf("Setting frequency to 1 MHz...\n");
  spi.SetFrequency(1000000);

  printf("Performing SPI transfer...\n");
  std::vector<uint8_t> tx_data = {0x01, 0x02, 0x03, 0x04,
                                  0x05, 0x06, 0x07, 0x08};
  std::vector<uint8_t> rx_data;

  spi.Transfer(tx_data, rx_data);

  printf("\nTest complete!\n");

  while (true) {
    ThisThread::sleep_for(1s);
  }

  return 0;
}
