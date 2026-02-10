#include <NanoHW/system.hpp>
#include <iostream>

void nano_hw::LockSleepImpl() {
  std::cout << "LockSleepImpl called" << std::endl;
}
void nano_hw::UnlockSleepImpl() {
  std::cout << "UnlockSleepImpl called" << std::endl;
}