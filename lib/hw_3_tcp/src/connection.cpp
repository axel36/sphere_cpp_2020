#include "tcp.hpp"

#include "logger.hpp"
#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

namespace tcp {

Connection::Connection(Connection &&other) noexcept
    : socket_(std::move(other.socket_)) {

  std::swap(addr_, other.addr_);
  std::swap(port_, other.port_);
  std::swap(timeout_, other.timeout_);
}

Connection &Connection::operator=(Connection &&other) noexcept {
  if (this != &other) {
    socket_ = std::move(other.socket_);

    std::swap(addr_, other.addr_);
    std::swap(port_, other.port_);
    std::swap(timeout_, other.timeout_);
  }
  return *this;
}

Connection::Connection(desc::Descriptor &&client_socket, std::string addr,
                       uint16_t port)
    : socket_(std::move(client_socket)), addr_(std::move(addr)), port_(port) {}

void Connection::Close() {
  log::INFO("close connection on fd = " + std::to_string(*socket_));
  socket_.close();
  timeout_ = std::nullopt;
}

size_t Connection::Read(char *data, size_t len) {
  if (!socket_.isValid()) {
    log::ERROR("try to read from invalid descriptor");
    throw ConnectionError("can't read: invalid descriptor");
  }
  ssize_t bytes_read = ::read(*socket_, data, len);

  if (bytes_read < 0) {
    LogErrorAndThrow();
  }

  if (bytes_read == 0) {
    log::INFO("other side close connection on fd= " + std::to_string(*socket_));
    socket_.close();
  }

  log::DEBUG("from socket " + std::to_string(*socket_) +
             ", read bytes = " + std::to_string(bytes_read));
  return static_cast<size_t>(bytes_read);
}

void Connection::ReadExact(char *data, size_t len) {
  size_t rest = len;
  size_t position = 0;
  size_t bytes_read = 0;
  while ((bytes_read = Read(static_cast<char *>(data) + position, rest)) > 0) {
    if (bytes_read >= rest) {
      return;
    }
    rest -= bytes_read;
    position += bytes_read;
  }
  if (bytes_read == 0) {
    log::WARN("channel was closed while reading, rest_bytes= " +
              std::to_string(rest));
    throw ConnectionError("channel was closed while reading");
  }
}

size_t Connection::Write(const char *data, size_t len) {
  if (!socket_.isValid()) {
    log::ERROR("can't write message: invalid descriptor");
    throw ConnectionError("can't write message: invalid descriptor");
  }
  ssize_t bytes_wrote = ::write(*socket_, data, len);

  if (bytes_wrote < 0) {
    LogErrorAndThrow();
  }
  if (bytes_wrote == 0) {
    log::INFO("other side close connection on fd= " + std::to_string(*socket_));
    socket_.close();
  }
  log::DEBUG("to socket " + std::to_string(*socket_) +
             ", wrote bytes = " + std::to_string(bytes_wrote));
  return static_cast<size_t>(bytes_wrote);
}

void Connection::WriteExact(const char *data, size_t rest) {
  size_t position = 0;
  size_t bytes_write = 0;
  while ((bytes_write =
              Write(static_cast<const char *>(data) + position, rest)) > 0) {
    if (bytes_write >= rest) {
      return;
    }
    rest -= bytes_write;
    position += bytes_write;
  }
  if (bytes_write == 0) {
    log::WARN("channel was closed while writing, rest_bytes= " +
              std::to_string(rest));
    throw ConnectionError("channel was closed while writing");
  }
}

void Connection::SetTimeout(size_t sec, size_t ms) {
  timeval timeout{};
  timeout.tv_sec = sec;
  timeout.tv_usec = ms;

  SetTimeout(timeout);
}

void Connection::SetTimeout(const timeval &timeout) {
  if (!socket_.isValid()) {
    timeout_ = timeout;
    return;
  }
  SetTimeoutInternal(timeout);
}

void Connection::SetTimeoutInternal(const timeval &timeout) {

  assert(socket_.isValid());

  log::DEBUG("try to set timeout s= " + std::to_string(timeout.tv_sec) +
             ", ms= " + std::to_string(timeout.tv_usec));

  if (::setsockopt(*socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout,
                   sizeof(timeout)) < 0) {
    log::WARN("can't set out timeout to socket: " + std::to_string(*socket_));
  }
  if (::setsockopt(*socket_, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                   sizeof(timeout)) < 0) {
    log::WARN("can't set in timeout to socket: " + std::to_string(*socket_));
  }
}

bool Connection::IsOpen() const { return socket_.isValid(); }

void Connection::LogErrorAndThrow() {

  socket_.close();
  log::ERROR(std::strerror(errno));
  throw ConnectionError(std::strerror(errno));
}

int Connection::GetSocket() const { return *socket_; }

std::string Connection::GetAddr() const { return addr_; }

std::string Connection::GetPort() const { return std::to_string(port_); }

ClientConnection::ClientConnection(const std::string &addr, int port) {
  addr_ = addr;
  port_ = port;
  Connect(addr, port);
}

void ClientConnection::Connect(const std::string &addr, int port) {

  if (socket_.isValid()) {
    log::DEBUG("try to create new connection on p=" + std::to_string(port) +
               ", addr= " + addr + "; close old one on ");
    Close();
  } else {
    log::DEBUG("try to create new connection on p=" + std::to_string(port) +
               ", addr= " + addr);
  }

  try {
    socket_ = desc::Descriptor(::socket(AF_INET, SOCK_STREAM, 0));
  } catch (const desc::DescriptorError &err) {
    LogErrorAndThrow();
  }

  if (timeout_) {
    SetTimeoutInternal(*timeout_);
  }

  sockaddr_in sock_addr{};
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = ::htons(port);
  int result = ::inet_aton(addr.c_str(), &sock_addr.sin_addr);
  if (result <= 0) {
    LogErrorAndThrow();
  }

  result = ::connect(*socket_, reinterpret_cast<sockaddr *>(&sock_addr),
                     sizeof(sock_addr));
  if (result != 0) {
    LogErrorAndThrow();
  }
}

} // namespace tcp
