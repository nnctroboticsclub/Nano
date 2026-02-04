#pragma once

#include <concepts>
#include <cstdint>
#include <vector>

#include <NanoHW/pin.hpp>
#include <NanoHW/policies.hpp>
#include "pin.hpp"

namespace nano_hw::spi {

enum class SPIFormat : uint8_t {
  Mode0,
  Mode1,
  Mode2,
  Mode3,
};

template <typename T>
concept SPIConfig =
    Policy<typename T::OnTransfer, void*, std::vector<uint8_t> const&,
           std::vector<uint8_t> const&>;

struct DummySPIConfig {
  using OnTransfer = nano_hw::Ignore;
};
static_assert(SPIConfig<DummySPIConfig>);

template <template <SPIConfig> typename SPIT>
concept SPI = requires(SPIT<DummySPIConfig> spi, Pin miso, Pin mosi, Pin sclk,
                       int frequency, std::vector<uint8_t> const& tx,
                       std::vector<uint8_t>& rx) {
  {SPIT<DummySPIConfig>(miso, mosi, sclk, frequency)}
      ->std::same_as<SPIT<DummySPIConfig>>;
  {spi.SetMode(SPIFormat::Mode0)}->std::same_as<void>;
  {spi.SetFrequency(frequency)}->std::same_as<void>;
  {spi.Transfer(tx, rx)}->std::same_as<int>;
};

struct ICallbacks {
 public:
  virtual ~ICallbacks() = default;
  virtual void OnTransfer(void* context, std::vector<uint8_t> const& tx,
                          std::vector<uint8_t> const& rx) = 0;
};

void* AllocInterface(Pin miso, Pin mosi, Pin sclk, int frequency,
                     ICallbacks* callbacks, void* callback_context);
void FreeInterface(void* interface);
void SetModeImpl(void* interface, SPIFormat format);
void SetFrequencyImpl(void* interface, int frequency);
int TransferImpl(void* interface, std::vector<uint8_t> const& tx_buffer,
                 std::vector<uint8_t>& rx_buffer);

template <SPIConfig Config>
class DynSPI {
  struct Callbacks : public ICallbacks {
   public:
    ~Callbacks() override = default;
    void OnTransfer(void* context, std::vector<uint8_t> const& tx,
                    std::vector<uint8_t> const& rx) final {
      Config::OnTransfer::execute(context, tx, rx);
    }
  };

 public:
  explicit DynSPI(Pin miso, Pin mosi, Pin sclk, int frequency)
      : interface_(
            AllocInterface(miso, mosi, sclk, frequency, &callbacks, this)) {}

  explicit DynSPI(Pin miso, Pin mosi, Pin sclk, int frequency,
                  void* callback_context)
      : interface_(AllocInterface(miso, mosi, sclk, frequency, &callbacks,
                                  callback_context)) {}

  ~DynSPI() { FreeInterface(interface_); }

  void SetMode(SPIFormat format) { SetModeImpl(interface_, format); }
  void SetFrequency(int frequency) { SetFrequencyImpl(interface_, frequency); }
  int Transfer(std::vector<uint8_t> const& tx_buffer,
               std::vector<uint8_t>& rx_buffer) {
    return TransferImpl(interface_, tx_buffer, rx_buffer);
  }

 private:
  static inline Callbacks callbacks = {};
  void* interface_;
};

static_assert(SPI<DynSPI>);

}  // namespace nano_hw::spi