#include <cstring>
#include <iostream>
#include <wait.h>
#include <zconf.h>

#include "process.hpp"

namespace proc {
Process::Process(const std::string &path) {

  int parent_to_child_pipe[2];
  int child_to_parent_pipe[2];

  if (pipe(parent_to_child_pipe) < 0) {
    throw std::runtime_error("can't create pipe");
  }
  desc::Descriptor to_child_write{parent_to_child_pipe[1]};
  desc::Descriptor to_child_read{parent_to_child_pipe[0]};

  if (pipe(child_to_parent_pipe) < 0) {
    throw std::runtime_error("can't create pipe");
  }
  desc::Descriptor to_parent_write{child_to_parent_pipe[1]};
  desc::Descriptor to_parent_read{child_to_parent_pipe[0]};

  fork_pid_ = fork();
  if (fork_pid_ < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }
  if (fork_pid_ == 0) {
    dup2(*to_parent_write, STDOUT_FILENO);
    to_parent_write.close();
    to_parent_read.close();

    dup2(*to_child_read, STDIN_FILENO);
    to_child_read.close();
    to_child_write.close();

    execlp(path.data(), path.data(), nullptr);
    std::cerr << "can't run command execlp with path " << path.data()
              << std::endl;
    exit(1);
  }

  read_from_child_ = std::move(to_parent_read);
  write_to_child_ = std::move(to_child_write);
}

Process::~Process() {
  try {
    close();
  } catch (const ProcessError &exc) {
  } catch (const std::exception &exc) {
    std::cerr << exc.what() << std::endl;
  }
}

size_t Process::read(void *data, size_t len) {
  if (!read_from_child_.isValid()) {
    throw std::runtime_error("can't read message: channel was closed");
  }
  ssize_t bytes_read = ::read(*read_from_child_, data, len);

  if (bytes_read < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }

  if (bytes_read == 0) {
    read_from_child_ = desc::Descriptor(-1);
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
  if (bytes_read == 0) {
    throw std::runtime_error("channel was closed while reading");
  }
}

size_t Process::write(const void *data, size_t len) {
  if (!write_to_child_.isValid()) {
    throw std::runtime_error("can't write message: channel was closed");
  }
  ssize_t bytes_write = ::write(*write_to_child_, data, len);

  if (bytes_write < 0) {
    throw std::runtime_error(std::string(strerror(errno)));
  }
  if (bytes_write == 0) {
    write_to_child_ = desc::Descriptor(-1);
  }

  return static_cast<size_t>(bytes_write);
}

void Process::writeExact(const void *data, size_t rest) {
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
  if (bytes_write == 0) {
    throw std::runtime_error("channel was closed while writing");
  }
}

void Process::closeStdin() { write_to_child_.closeThrow(); }

void Process::close() {
  if (isProcessRunning()) {

    if (write_to_child_.isValid()) {
      write_to_child_.close();
    }
    if (read_from_child_.isValid()) {
      read_from_child_.close();
    }

    kill(fork_pid_, SIGINT);
    waitpid(fork_pid_, nullptr, 0);
    fork_pid_ = -1;
  } else {
    throw ProcessError("error: can't end Process twice");
  }
}

bool Process::isWriteChannelOpen() const { return write_to_child_.isValid(); }

bool Process::isReadChannelOpen() const { return read_from_child_.isValid(); }

bool Process::isProcessRunning() const { return fork_pid_ > 0; }

} // namespace proc
