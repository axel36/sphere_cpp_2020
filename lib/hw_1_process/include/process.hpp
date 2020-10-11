#pragma once
#include <string>

namespace proc {
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
  int read_from_child_fd_ = -1;
  int write_to_child_fd_ = -1;
  pid_t fork_pid_ = -1;
};
} // namespace proc
