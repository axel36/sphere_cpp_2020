#pragma once

#include "descriptor.hpp"
#include <string>

namespace proc {
class ProcessError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class Process {
public:
  explicit Process(const std::string &path);
  ~Process();

  size_t write(const void *data, size_t len);
  void writeExact(const void *data, size_t len);
  size_t read(void *data, size_t len);
  void readExact(void *data, size_t len);

  void closeStdin();
  void close();

  [[nodiscard]] bool isWriteChannelOpen() const;
  [[nodiscard]] bool isReadChannelOpen() const;
  [[nodiscard]] bool isProcessRunning() const;

private:
  desc::Descriptor read_from_child_;
  desc::Descriptor write_to_child_;
  pid_t fork_pid_ = -1;
};
} // namespace proc
