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
    throw std::runtime_error("can't create pipe");
  }
  if (pipe(child_to_parent_pipe) < 0) {
    throw std::runtime_error("can't create pipe");
  }

  fork_pid_ = fork();
  if (fork_pid_ < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }
  if (fork_pid_ == 0) {
    dup2(child_to_parent_pipe[1], STDOUT_FILENO);
    ::close(child_to_parent_pipe[1]);
    ::close(child_to_parent_pipe[0]);

    dup2(parent_to_child_pipe[0], STDIN_FILENO);
    ::close(parent_to_child_pipe[1]);
    ::close(parent_to_child_pipe[0]);

    execlp(path.data(), path.data(), nullptr);
    std::cerr << "can't run command execlp with path " << path.data()
              << std::endl;
    exit(1);
  }

  ::close(parent_to_child_pipe[0]);
  ::close(child_to_parent_pipe[1]);

  read_from_child_fd_ = child_to_parent_pipe[0];
  write_to_child_fd_ = parent_to_child_pipe[1];
}

Process::~Process() {
  if (isProcessRunning()) {
    close();
  }
}

size_t Process::read(void *data, size_t len) {
  if (read_from_child_fd_ <= 0) {
    throw std::runtime_error("can't read message: channel was closed");
  }
  long int bytes_read = ::read(read_from_child_fd_, data, len);
  if (bytes_read == 0) {
    read_from_child_fd_ = -1;
    throw std::runtime_error("channel was closed while reading");
  }
  if (bytes_read < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }
  return static_cast<size_t>(bytes_read);
}

void Process::readExact(void *data, size_t len) {
  size_t rest = len;
  size_t position = 0;
  size_t bytes_read = 0;
  while ((bytes_read = read(static_cast<char *>(data) + position, rest)) > 0) {
    if (bytes_read >= rest) {
      return;
    }
    rest -= bytes_read;
    position += bytes_read;
  }
}

size_t Process::write(const void *data, size_t len) {
  if (!isChannelOpen()) {
    throw std::runtime_error("can't write message: channel was closed");
  }
  long int bytes_write = ::write(write_to_child_fd_, data, len);

  if (bytes_write == 0) {
    write_to_child_fd_ = -1;
    throw std::runtime_error("channel was closed while writing");
  }
  if (bytes_write < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }

  return static_cast<size_t>(bytes_write);
}

void Process::writeExact(const void *data, size_t len) {
  if (!isChannelOpen()) {
    throw std::runtime_error("can't write message: channel was closed");
  }
  size_t rest = len;
  size_t position = 0;
  size_t bytes_write = 0;
  while ((bytes_write =
              write(static_cast<const char *>(data) + position, rest)) > 0) {
    if (bytes_write >= rest) {
      return;
    }
    rest -= bytes_write;
    position += bytes_write;
  }
}

void Process::closeStdin() {
  if (isChannelOpen()) {
    ::close(write_to_child_fd_);
    write_to_child_fd_ = -1;
  } else {
    std::cout << "error can't close stdin twice" << std::endl;
  }
}

void Process::close() {
  if (isProcessRunning()) {

    if (isChannelOpen()) {
      closeStdin();
    }

    ::close(read_from_child_fd_);
    read_from_child_fd_ = -1;

    kill(fork_pid_, SIGINT);
    waitpid(fork_pid_, nullptr, 0);
    fork_pid_ = -1;
  } else {
    std::cout << "error: can't end Process twice" << std::endl;
  }
}
} // namespace proc
