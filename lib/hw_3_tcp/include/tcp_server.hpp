#pragma once

#include "descriptor.hpp"
#include "tcp.hpp"
#include <iostream>
#include <optional>

namespace tcp {

class ServerError : public TcpError {
  using TcpError::TcpError;
};

class ServerAcceptError : public ServerError {
  using ServerError::ServerError;
};

class Server {
public:
  Server() = default;
  Server(const std::string &addr, int port, size_t max_connections);

  Server(Server &other) = delete;
  Server(Server &&other) noexcept;

  Server &operator=(Server &other) = delete;
  Server &operator=(Server &&other) noexcept;

  void Open(const std::string &addr, int port, size_t max_connections);
  void Close();
  Connection Accept();

  void SetTimeout(size_t sec, size_t ms);

private:
  desc::Descriptor server_socket_;
  std::optional<timeval> timeout_;

  void LogErrorAndThrow();
  void SetTimeoutInternal(const timeval &timeout);
};

} // namespace tcp
