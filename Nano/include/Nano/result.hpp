#pragma once

#include <optional>
#include <stdexcept>
#include <type_traits>
#include <variant>

namespace robotics::inline types {
template <typename E>
struct ResultErrorType {
  E error;
};

template <typename T>
struct ResultValueType {
  T value;
};

template <typename T, typename E>
class Result {
  using ValueT = std::conditional_t<std::is_void_v<T>, std::monostate, T>;
  using ErrorT = std::conditional_t<std::is_void_v<E>, std::monostate, E>;

  static_assert(!std::is_void_v<T> || !std::is_void_v<E>,
                "Both T and E cannot be void.");

  enum class Tag : bool {
    kOk,
    kError,
  };

  Tag tag_;

  ValueT value_;
  ErrorT error_;

 public:
  Result(T value) = delete;
  Result(E error) = delete;

  explicit(false) Result(ResultErrorType<E> error)
      : tag_(Tag::kError), error_(error.error) {}

  explicit(false) Result(ResultValueType<T> value)
      : tag_(Tag::kOk), value_(value.value) {}

  Result() = default;

  std::optional<T> Value() const requires(!std::is_void_v<T>) {
    if (tag_ == Tag::kError) {
      return std::nullopt;
    }
    return value_;
  }

  std::optional<E> Error() const {
    if (tag_ == Tag::kOk) {
      return std::nullopt;
    }
    return error_;
  }

  [[nodiscard]] bool IsOk() const { return tag_ == Tag::kOk; }
};

template <typename T>
static ResultValueType<T> Ok(T value) requires(!std::is_void_v<T>) {
  return ResultValueType{value};
}

template <std::same_as<void> T>
static ResultValueType<T> Ok() {
  return ResultValueType<T>{};
}

template <typename E>
static ResultErrorType<E> Err(E error) {
  return ResultErrorType<E>{error};
}

}  // namespace robotics::inline types
