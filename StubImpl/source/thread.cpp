#include <NanoHW/thread.hpp>

#include <functional>
#include <iostream>

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

void* nano_hw::thread::AllocInterface(ThreadPriority priority,
                                      uint32_t stack_size,
                                      unsigned char* stack_mem,
                                      const char* name) {
  return new MockThread(priority, stack_size, stack_mem, name);
}

void nano_hw::thread::FreeInterface(void* interface) {
  delete static_cast<MockThread*>(interface);
}

void nano_hw::thread::StartImpl(void* interface, std::function<void()> task) {
  static_cast<MockThread*>(interface)->Start(task);
}

void nano_hw::thread::JoinImpl(void* interface) {
  static_cast<MockThread*>(interface)->Join();
}

void nano_hw::thread::TerminateImpl(void* interface) {
  static_cast<MockThread*>(interface)->Terminate();
}

void nano_hw::thread::SetPriorityImpl(void* interface,
                                      ThreadPriority priority) {
  static_cast<MockThread*>(interface)->SetPriority(priority);
}

ThreadPriority nano_hw::thread::GetPriorityImpl(void* interface) {
  return static_cast<MockThread*>(interface)->GetPriority();
}
