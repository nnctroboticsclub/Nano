#pragma once

#include <cstddef>
#include <functional>
#include "Nano/no_mutex_lifo.hpp"

namespace {
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
  Callback(T* obj, R (T::*method)(Args...))
      : _func([obj, method](Args... args) { return (obj->*method)(args...); }) {

  }

  explicit(false) Callback(R (*func)(Args...)) : _func(func) {}

  explicit(false) Callback(std::function<R(Args...)> func) : _func(func) {}

  // Lambda support
  template <typename F>
      requires std::is_invocable_r_v<R, F, Args...> &&
      (!std::is_same_v<std::decay_t<F>, Callback<R(Args...)>>) &&
      (!std::is_function_v<std::remove_pointer_t<std::decay_t<F>>>)explicit(
          false) Callback(F&& lambda)
      : _func(std::forward<F>(lambda)) {}

  R operator()(Args... args) { return _func(args...); }

  // NOLINTNEXTLINE
  operator bool() const { return _func != nullptr; }

 private:
  std::function<R(Args...)> _func;
};

}  // namespace mbed
}  // namespace