#include "Process.hpp"
#include <cstring>
#include <iostream>
#include <wait.h>
#include <zconf.h>

namespace proc {
Process::Process(const std::string &path) {

  int parent_to_child_pipe[2];
  int child_to_parent_pipe[2];

  if (pipe(parent_to_child_pipe) < 0) {
  }
  pipe(child_to_parent_pipe);

  fork_pid_ = fork();
  if (fork_pid_ == 0) {
    dup2(child_to_parent_pipe[1], STDOUT_FILENO);
    ::close(child_to_parent_pipe[1]);
    ::close(child_to_parent_pipe[0]);

    dup2(parent_to_child_pipe[0], STDIN_FILENO);
    ::close(parent_to_child_pipe[1]);
    ::close(parent_to_child_pipe[0]);

    execlp(path.data(), path.data(), nullptr);
    raise(SIGINT);
    return;
  }

  ::close(parent_to_child_pipe[0]);
  ::close(child_to_parent_pipe[1]);

  read_from_child_fd = child_to_parent_pipe[0];
  write_to_child_fd = parent_to_child_pipe[1];
}

Process::~Process() {
  ::close(read_from_child_fd);
  close();
}

size_t Process::read(void *data, size_t len) {
  return ::read(read_from_child_fd, data, len);
}

void Process::readExact(void *data, size_t len) {

  int rest = len;
  int position = 0;
  int bytes_read = -1;
  char buffer[len];

  while ((bytes_read = read(buffer + position, rest)) > 0) {
    if (bytes_read >= rest) {
      std::memmove(data, buffer, len);
      return;
    }
    rest -= bytes_read;
    position += bytes_read;
  }
  if (bytes_read == 0) {
    throw std::runtime_error(
        "can't read whole message: channel was closed while reading");
  } else {
    throw std::runtime_error(std::string(strerror(errno)));
  }
}

void Process::writeExact(const void *data, size_t len) {
  if (!isChannelOpen()) {
    throw std::runtime_error("can't write message: channel was closed");
  }

  int rest = len;
  int position = 0;
  int bytes_write = -1;
  while ((bytes_write = write(reinterpret_cast<const char *>(data) + position,
                              rest)) > 0) {
    if (bytes_write >= rest) {
      return;
    }
    rest -= bytes_write;
    position += bytes_write;
  }
  if (bytes_write == 0) {
    channel_open_ = false;
    throw std::runtime_error(
        "can't write whole message: channel was closed while writing");
  } else {
    throw std::runtime_error(std::string(strerror(errno)));
  }
}

size_t Process::write(const void *data, size_t len) {
  if (!isChannelOpen()) {
    throw std::runtime_error("can't write message: channel was closed");
  }
  return ::write(write_to_child_fd, data, len);
}

void Process::closeStdin() {
  if (isChannelOpen()) {
    ::close(write_to_child_fd);
    channel_open_ = false;
  } else {
    std::cout << "error can't close stdin twice" << std::endl;
  }
}

void Process::close() {
  if (isProcessRunning()) {

    if (isChannelOpen()) {
      closeStdin();
    }

    kill(fork_pid_, SIGINT);
    waitpid(fork_pid_, nullptr, 0);

    process_running_ = false;
  } else {
    std::cout << "error can't close Process twice" << std::endl;
  }
}
} // namespace proc
