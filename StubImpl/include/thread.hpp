#pragma once

#include <NanoHW/thread_impl.hpp>

#include <functional>
#include <iostream>

namespace nano_stub {
using ::ThreadPriority;

class MockThread {
 public:
  MockThread(ThreadPriority priority, uint32_t stack_size,
             unsigned char* stack_mem, const char* name)
      : priority_(priority),
        stack_size_(stack_size),
        stack_mem_(stack_mem),
        name_(name ? name : "unnamed"),
        started_(false),
        terminated_(false) {
    (void)stack_mem_;
    std::cout << "MockThread initialized: name=" << name_
              << ", priority=" << priority_ << ", stack_size=" << stack_size_
              << "\n";
  }

  void Start(std::function<void()> task) {
    std::cout << "MockThread Start: " << name_ << "\n";
    started_ = true;

    // Mock execution - immediately execute task
    if (task) {
      task();
    }
  }

  void Join() {
    std::cout << "MockThread Join: " << name_ << "\n";
    started_ = false;
  }

  void Terminate() {
    std::cout << "MockThread Terminate: " << name_ << "\n";
    terminated_ = true;
    started_ = false;
  }

  void SetPriority(ThreadPriority priority) {
    std::cout << "MockThread SetPriority: " << name_
              << ", priority=" << priority << "\n";
    priority_ = priority;
  }

  ThreadPriority GetPriority() const { return priority_; }

 private:
  ThreadPriority priority_;
  uint32_t stack_size_;
  unsigned char* stack_mem_;
  std::string name_;
  bool started_;
  bool terminated_;
};
}  // namespace nano_stub