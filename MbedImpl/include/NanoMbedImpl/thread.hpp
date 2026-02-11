#pragma once

#include <functional>

#include <mbed.h>
#include <NanoHW/thread_impl.hpp>

namespace nano_mbed {
using ::ThreadPriority;

// Map NanoHW ThreadPriority to Mbed osPriority
static osPriority MapThreadPriority(ThreadPriority priority) {
  switch (priority) {
    case ThreadPriorityIdle:
      return osPriorityIdle;
    case ThreadPriorityLow:
      return osPriorityLow;
    case ThreadPriorityBelowNormal:
      return osPriorityBelowNormal;
    case ThreadPriorityNormal:
      return osPriorityNormal;
    case ThreadPriorityAboveNormal:
      return osPriorityAboveNormal;
    case ThreadPriorityHigh:
      return osPriorityHigh;
    case ThreadPriorityRealtime:
      return osPriorityRealtime;
    default:
      return osPriorityNormal;
  }
}

class MbedThread {
 public:
  MbedThread(ThreadPriority priority, uint32_t stack_size,
             unsigned char* stack_mem, const char* name)
      : priority_(priority),
        stack_size_(stack_size),
        stack_mem_(stack_mem),
        name_(name),
        task_(nullptr),
        thread_(nullptr),
        terminated_(false) {}

  ~MbedThread() {
    if (thread_ != nullptr) {
      thread_->terminate();
      delete thread_;
    }
  }

  void Start(std::function<void()> task) {
    if (!task || thread_ != nullptr)
      return;

    // Store the task for execution
    task_ = task;

    // Create a Mbed RTOS thread with the specified parameters
    thread_ = new rtos::Thread(MapThreadPriority(priority_),
                               stack_size_ > 0 ? stack_size_ : OS_STACK_SIZE,
                               stack_mem_, name_);

    // Start the thread with a member function callback
    thread_->start(mbed::callback(this, &MbedThread::RunTask));
  }

  void Join() {
    if (thread_ != nullptr) {
      thread_->join();
    }
  }

  void Terminate() {
    terminated_ = true;
    if (thread_ != nullptr) {
      thread_->terminate();
    }
  }

  void SetPriority(ThreadPriority priority) {
    priority_ = priority;
    if (thread_ != nullptr) {
      thread_->set_priority(MapThreadPriority(priority));
    }
  }

  ThreadPriority GetPriority() const { return priority_; }

 private:
  void RunTask() {
    if (task_) {
      task_();
    }
  }

  ThreadPriority priority_;
  [[maybe_unused]] uint32_t stack_size_;
  [[maybe_unused]] unsigned char* stack_mem_;
  [[maybe_unused]] const char* name_;
  std::function<void()> task_;
  rtos::Thread* thread_;
  bool terminated_;
};
}  // namespace nano_mbed