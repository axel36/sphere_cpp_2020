#pragma once
#include <string>

namespace proc{
class Process
{
public:
  explicit Process(const std::string& path);
  ~Process();

  size_t write(const void* data, size_t len);
  void writeExact(const void* data, size_t len);
  size_t read(void* data, size_t len);
  void readExact(void* data, size_t len);

  void closeStdin();
  void close();

private:
  int parent_to_child_pipe_[2];
  int child_to_parent_pipe_[2];
  pid_t fork_pid_;
};
}

