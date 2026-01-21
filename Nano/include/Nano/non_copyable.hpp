#pragma once

namespace Nano::utils {
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
}  // namespace Nano::utils