#pragma once

#include <cstddef>
#include <type_traits>

namespace Nano::collection {
template <typename T>
class Span {
 public:
  Span(T* data, size_t size) : data_(data), size_(size) {}
  Span() : data_(nullptr), size_(0) {}

  // copy
  Span(Span<T>& other) = default;

  template <typename U>
  explicit(false)
      Span(const Span<U>& other) requires std::is_convertible_v<U*, T*>
      : data_(other.data()), size_(other.size()) {}

  T* data() const { return data_; }
  [[nodiscard]] size_t size() const { return size_; }

  T& operator[](size_t index) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-pointer-arithmetic)
    return data_[index];
  }

 private:
  T* data_;
  size_t size_;
};
}  // namespace Nano::collection