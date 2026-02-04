#pragma once

#include <cstddef>
#include <functional>
#include "Nano/no_mutex_lifo.hpp"
namespace mbed {
template <typename T, size_t N>
class CircularBuffer {
  Nano::collection::NoMutexLIFO<T, N> buffer;

 public:
  void push(T value) { buffer.Push(std::move(value)); }
  [[nodiscard]] bool empty() const { return buffer.Empty(); }
  void pop(T& value) { value = buffer.Pop(); }
};

template <typename T>
class Callback {};

// Callback<void()>(T* obj, void (T::*method)())
// Callback<void()>(void (*func)())
// Callback<void()>(std::function<void()> func)
template <typename R, typename... Args>
class Callback<R(Args...)> {
 public:
  Callback() : _func(nullptr) {}

  template <typename T>
  Callback(T* obj, R (T::*method)(Args...)) {
    _func = [obj, method](Args... args) {
      return (obj->*method)(args...);
    };
  }

  Callback(R (*func)(Args...)) { _func = func; }

  Callback(std::function<R(Args...)> func) : _func(func) {}

  R operator()(Args... args) { return _func(args...); }

  operator bool() const { return _func != nullptr; }

 private:
  std::function<R(Args...)> _func;
};

}  // namespace mbed