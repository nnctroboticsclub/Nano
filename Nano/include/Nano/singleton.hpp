#pragma once

namespace nano::utils {
template <typename T>
class Singleton {
 public:
  Singleton(Singleton const&) = delete;
  Singleton& operator=(Singleton const&) = delete;

  static T& GetInstance() {
    static T instance;
    return instance;
  }

 protected:
  // Hidden 'Singleton' constructor
  Singleton() = default;
  ~Singleton() = default;
};
}  // namespace nano::utils