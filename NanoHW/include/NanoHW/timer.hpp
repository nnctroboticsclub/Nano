#pragma once

#include <chrono>
#include <concepts>

#include <NanoHW/policies.hpp>

namespace nano_hw::timer {

template <typename T>
concept TimerConfig = Policy<typename T::OnTick, void*>;

struct DummyTimerConfig {
  using OnTick = nano_hw::Ignore;
};
static_assert(TimerConfig<DummyTimerConfig>);

template <template <TimerConfig> typename TimerT>
concept Timer = requires(TimerT<DummyTimerConfig> value) {
  {TimerT<DummyTimerConfig>()}->std::same_as<TimerT<DummyTimerConfig>>;

  {value.Reset()}->std::same_as<void>;
  {value.Start()}->std::same_as<void>;
  {value.Stop()}->std::same_as<void>;
  {value.Read()}->std::same_as<std::chrono::milliseconds>;
  {value.EnableTick(std::chrono::milliseconds(100))}->std::same_as<bool>;
};

// Callback interface with instance context support
struct ICallbacks {
  virtual void OnTick(void* context) = 0;
};

void* AllocInterface(ICallbacks* callbacks, void* callback_context);
void FreeInterface(void* interface);
void ResetImpl(void* interface);
void StartImpl(void* interface);
void StopImpl(void* interface);
std::chrono::milliseconds ReadImpl(void* interface);
bool EnableTickImpl(void* interface, std::chrono::milliseconds interval);

// Implementation must be in header for inline

template <TimerConfig Config>
class DynTimer {
  struct Callbacks : public ICallbacks {
   public:
    void OnTick(void* context) final { Config::OnTick::execute(context); }
  };

 public:
  explicit DynTimer() : interface_(AllocInterface(&callbacks, this)) {}

  explicit DynTimer(void* callback_context)
      : interface_(AllocInterface(&callbacks, callback_context)) {}

  ~DynTimer() { FreeInterface(interface_); }

  void Reset() { ResetImpl(interface_); }
  void Start() { StartImpl(interface_); }
  void Stop() { StopImpl(interface_); }
  std::chrono::milliseconds Read() const { return ReadImpl(interface_); }
  bool EnableTick(std::chrono::milliseconds interval) {
    return EnableTickImpl(interface_, interval);
  }

 private:
  static Callbacks callbacks;
  void* interface_;
};

template <TimerConfig Config>
typename DynTimer<Config>::Callbacks DynTimer<Config>::callbacks;

static_assert(Timer<DynTimer>);

}  // namespace nano_hw::timer
