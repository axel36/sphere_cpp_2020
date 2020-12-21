#pragma once

#include "descriptor.hpp"
#include <iostream>
#include <optional>

namespace tcp {

class TcpError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class ConnectionError : public TcpError {
  using TcpError::TcpError;
};

class ServerError : public TcpError {
  using TcpError::TcpError;
};

class Connection {
public:
  Connection() = default;
  Connection(desc::Descriptor &&client_socket, std::string addr, uint16_t port);

  Connection(const Connection &other) = delete;
  Connection(Connection &&other) noexcept;

  Connection &operator=(const Connection &other) = delete;
  Connection &operator=(Connection &&other) noexcept;

  virtual ~Connection() = default;

  void Close();

  size_t Read(char *, size_t);
  void ReadExact(char *, size_t);

  size_t Write(const char *, size_t);
  void WriteExact(const char *, size_t);

  void SetTimeout(size_t sec, size_t ms);
  void SetTimeout(const timeval &timeout);

  [[nodiscard]] bool IsOpen() const;
  [[nodiscard]] int GetSocket() const;
  [[nodiscard]] std::string GetAddr() const;
  [[nodiscard]] std::string GetPort() const;
  [[nodiscard]] std::string GetInfo() const;
  void set(int f){
    port_= f;
  }
protected:
  desc::Descriptor socket_;
  std::optional<timeval> timeout_;
  std::string addr_;
  uint16_t port_;

  void SetTimeoutInternal(const timeval &timeout);
  void LogErrorAndThrow();
};

class ClientConnection : public Connection {
public:
  ClientConnection(const std::string &addr, int port);
  void Connect(const std::string &addr, int port);
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
