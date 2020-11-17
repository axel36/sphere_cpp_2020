#pragma once

#include <stdexcept>

namespace desc {

class DescriptorError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class Descriptor {
public:
  Descriptor() = default;
  explicit Descriptor(int fd);
  Descriptor(const Descriptor &other) = delete;
  Descriptor(Descriptor &&other) noexcept;
  Descriptor &operator=(const Descriptor &) = delete;
  Descriptor &operator=(Descriptor &&other) noexcept;
  ~Descriptor() noexcept;

  void closeThrow();
  void close();
  [[nodiscard]] bool isValid() const;
  int operator*() const;

private:
  int fd_ = -1;
};
} // namespace desc
