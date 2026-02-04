#pragma once

#include <concepts>

namespace nano_hw {

template <typename T, typename... Args>
concept Policy = requires(Args && ... args) {
  {T::execute(static_cast<Args&&>(args)...)}->std::same_as<void>;
};

struct Ignore {
  template <typename... Args>
  static __attribute__((always_inline)) void execute(Args&&...) {}
};

template <auto Action>
struct Direct {
  template <typename... Args>
  static __attribute__((always_inline)) void execute(Args&&... args) {
    Action(static_cast<Args&&>(args)...);
  }
};

}  // namespace nano_hw