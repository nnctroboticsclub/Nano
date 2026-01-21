#pragma once

#include <array>
#include <cstddef>

namespace Nano::utils {

class ScratchBuffer {
  static constexpr const std::size_t kBufferSize = 0x80;
  static std::array<char, kBufferSize> buffer_isr_;
  static std::array<char, kBufferSize> buffer_app_;

 public:
  /// @brief ISR 用のバッファを取得する
  static void* GetISRBuffer() { return buffer_isr_.data(); }

  /// @brief アプリケーション用のバッファを取得する
  static void* GetAppBuffer() { return buffer_app_.data(); }
};

}  // namespace Nano::utils