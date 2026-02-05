#include <gtest/gtest.h>

#include <NanoMbed/thread.hpp>

using rtos::Thread;

TEST(ThreadTest, CreateThread) {
  Thread thread;
  SUCCEED();
}

TEST(ThreadTest, StartThread) {
  Thread thread;
  bool executed = false;

  thread.start([&executed]() { executed = true; });

  SUCCEED();
}

TEST(ThreadTest, SetPriority) {
  Thread thread;

  thread.set_priority(ThreadPriorityHigh);
  EXPECT_EQ(thread.get_priority(), ThreadPriorityHigh);

  thread.set_priority(ThreadPriorityLow);
  EXPECT_EQ(thread.get_priority(), ThreadPriorityLow);
}

TEST(ThreadTest, ThreadWithStackSize) {
  Thread thread(ThreadPriorityNormal, 8192);
  SUCCEED();
}

TEST(ThreadTest, TerminateThread) {
  Thread thread;
  thread.start([]() {});
  thread.terminate();
  SUCCEED();
}
