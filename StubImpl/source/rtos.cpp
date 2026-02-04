#include <NanoHW/parallel.hpp>
#include <iostream>
#include <thread>

namespace nano_hw::parallel {
void SleepForMS(std::chrono::milliseconds ms) {
  std::cout << "Sleeping for " << ms.count() << " milliseconds\n";

  std::this_thread::sleep_for(ms);
}
}  // namespace nano_hw::parallel