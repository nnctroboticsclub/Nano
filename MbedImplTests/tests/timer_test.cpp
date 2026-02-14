#include <mbed.h>
#include <NanoHW/timer.hpp>
#include <NanoMbedImpl/timer.hpp>

using namespace nano_hw;
using namespace nano_hw::timer;

int main() {
  printf("\n=== MbedImpl Timer Test ===\n");

  printf("Creating MbedTimer instance...\n");
  nano_mbed::MbedTimer<nano_hw::timer::DummyTimerConfig> timer(nullptr,
                                                               nullptr);

  printf("Starting timer...\n");
  timer.Start();

  printf("Waiting 1 second...\n");
  ThisThread::sleep_for(1s);

  auto elapsed = timer.Read();
  printf("Elapsed time: %lld ms\n", elapsed.count());

  printf("Waiting another 2 seconds...\n");
  ThisThread::sleep_for(2s);

  elapsed = timer.Read();
  printf("Total elapsed time: %lld ms\n", elapsed.count());

  printf("Stopping timer...\n");
  timer.Stop();

  printf("Resetting timer...\n");
  timer.Reset();

  elapsed = timer.Read();
  printf("Time after reset: %lld ms\n", elapsed.count());

  printf("\nTest complete!\n");

  while (true) {
    ThisThread::sleep_for(1s);
  }

  return 0;
}
