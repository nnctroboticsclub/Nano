#pragma once

namespace nano::utils {
/// @brief コピー不可を表すクラス
template <typename T>
class NonCopyable {
 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;

 public:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};
}  // namespace nano::utils