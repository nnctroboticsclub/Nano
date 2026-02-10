#pragma once

#include <cstddef>
#include <functional>
#include "Nano/no_mutex_lifo.hpp"

namespace {
namespace mbed::details {

// R(Args...) --> (T::R)(Args...) Utility type
template <typename T, typename Fn>
struct ClassMethod_Impl {
  using type = std::nullptr_t;
};

template <typename T, typename R, typename... Args>
struct ClassMethod_Impl<T, R(Args...)> {
  using type = R (T::*)(Args...);
};

template <typename T, typename Fn>
using ClassMethod = typename ClassMethod_Impl<T, Fn>::type;

}  // namespace mbed::details

namespace mbed {
template <typename T, size_t N>
class CircularBuffer {
  Nano::collection::NoMutexLIFO<T, N> buffer;

 public:
  void push(T value) { buffer.Push(std::move(value)); }
  [[nodiscard]] bool empty() const { return buffer.Empty(); }
  void pop(T& value) { value = buffer.Pop(); }
};

template <typename Fn>
class Callback : public std::function<Fn> {
 public:
  using std::function<Fn>::function;

  template <typename T, typename Method = mbed::details::ClassMethod<T, Fn>>
  Callback(T* obj, Method method) : std::function<Fn>(std::bind(method, obj)) {}
};

template <typename T, typename R, typename... Args>
Callback(T*, R (T::*)(Args...)) -> Callback<R(Args...)>;

// callback(T* obj, void (T::*method)()) -> Callback
// callback(R (*func)(Args...)) -> Callback
template <typename T>
auto callback(T* obj, void (T::*method)()) {
  return Callback<void()>(obj, method);
}

template <typename R, typename... Args>
auto callback(R (*func)(Args...)) {
  return Callback<R(Args...)>(func);
}

template <typename F>
auto callback(F&& lambda) {
  return Callback(std::forward<F>(lambda));
}

}  // namespace mbed
}  // namespace