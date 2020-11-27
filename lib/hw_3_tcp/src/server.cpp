#include "tcp_server.hpp"
#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <logger.hpp>
#include <netinet/in.h>

namespace tcp {

Server::Server(const std::string &addr, int port, size_t max_connections) {
  Open(addr, port, max_connections);
}
Server::Server(Server &&other) noexcept
    : server_socket_(std::move(other.server_socket_)),
      timeout_(other.timeout_) {

  other.timeout_.reset();
}
Server &Server::operator=(Server &&other) noexcept {
  if (this != &other) {
    server_socket_ = std::move(other.server_socket_);
    timeout_ = other.timeout_;
    other.timeout_.reset();
  }
  return *this;
}

void Server::Open(const std::string &addr, int port, size_t max_connections) {

  if (server_socket_.isValid()) {
    log::DEBUG("try to create new server, listen on p=" + std::to_string(port) +
               ", addr= " + addr + "; close old one on ");
    Close();
  } else {
    log::DEBUG("try to create new server, listen on p=" + std::to_string(port) +
               ", addr= " + addr);
  }

  try {
    server_socket_ = desc::Descriptor{::socket(AF_INET, SOCK_STREAM, 0)};
  } catch (const desc::DescriptorError &err) {
    LogErrorAndThrow();
  }

  int opt = 1;
  int result;
  result = ::setsockopt(*server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt,
                        sizeof(int));
  if (result < 0) {
    LogErrorAndThrow();
  }

  if (timeout_) {
    SetTimeoutInternal(*timeout_);
  }

  sockaddr_in sock_addr{};
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = ::htons(8080);
  sock_addr.sin_addr = {::htonl(INADDR_ANY)};

  result = ::bind(*server_socket_, reinterpret_cast<sockaddr *>(&sock_addr),
                  sizeof(sock_addr));
  if (result != 0) {
    LogErrorAndThrow();
  }

  result = ::listen(*server_socket_, max_connections);
  if (result != 0) {
    LogErrorAndThrow();
  }
}
void Server::Close() {

  log::INFO("close server on fd = " + std::to_string(*server_socket_));
  server_socket_.close();
  timeout_ = std::nullopt;
}

void Server::LogErrorAndThrow() {
  server_socket_.close();
  log::ERROR(std::strerror(errno));
  throw ConnectionError(std::strerror(errno));
}

Connection Server::Accept() {

  sockaddr_in client_sock_addr{};
  socklen_t s = sizeof(sockaddr_in);

  desc::Descriptor client_fd;
  try {
    client_fd = desc::Descriptor{::accept(
        *server_socket_, reinterpret_cast<sockaddr *>(&client_sock_addr), &s)};
  } catch (const desc::DescriptorError &err) {
    log::WARN("server_accept error: " + std::string(std::strerror(errno)));
    throw ServerAcceptError("server_accept error: " +
                            std::string(std::strerror(errno)));
  }

  std::string client_addr{::inet_ntoa(client_sock_addr.sin_addr)};
  log::INFO("get client from: addr= " + client_addr +
            ", p= " + std::to_string(client_sock_addr.sin_port));

  return {std::move(client_fd), client_addr, client_sock_addr.sin_port};
}

void Server::SetTimeout(size_t sec, size_t ms) {
  timeval timeout{};
  timeout.tv_sec = sec;
  timeout.tv_usec = ms;

  if (!server_socket_.isValid()) {
    timeout_ = timeout;
    return;
  }
  SetTimeoutInternal(timeout);
}

void Server::SetTimeoutInternal(const timeval &timeout) {

  assert(server_socket_.isValid());

  log::DEBUG("try to set timeout s= " + std::to_string(timeout.tv_sec) +
             ", ms= " + std::to_string(timeout.tv_usec));

  if (::setsockopt(*server_socket_, SOL_SOCKET, SO_SNDTIMEO, &timeout,
                   sizeof(timeout)) < 0) {
    log::WARN("can't set out timeout to socket: " +
              std::to_string(*server_socket_));
  }
  if (::setsockopt(*server_socket_, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                   sizeof(timeout)) < 0) {
    log::WARN("can't set in timeout to socket: " +
              std::to_string(*server_socket_));
  }
}

} // namespace tcp
