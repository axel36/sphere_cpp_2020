#pragma once

#include <stdexcept>

namespace desc{

class DescriptorError : public std::runtime_error{
public:
  explicit DescriptorError(const std::string& msg):std::runtime_error(msg){}
};

class Descriptor{
public:
  Descriptor() = default;
  explicit Descriptor(int fd);
  Descriptor(const Descriptor& other) = delete;
  Descriptor(Descriptor&& other) noexcept ;
  Descriptor& operator=(const Descriptor&) = delete;
  Descriptor& operator=(Descriptor&& other) noexcept ;
  ~Descriptor() noexcept;

  void close();
  bool isValid() const;

private:
  int fd_ = -1;

};
}
