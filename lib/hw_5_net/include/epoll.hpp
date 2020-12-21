#pragma once

#include "descriptor.hpp"
#include "tcp.hpp"

#include <list>

namespace net {

class NetError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class EpollError : public NetError {
  using NetError::NetError;
};

class Epoll {
public:
  Epoll();

  Epoll(Epoll &other) = delete;
  Epoll(Epoll &&other) noexcept;

  Epoll &operator=(Epoll &other) = delete;
  Epoll &operator=(Epoll &&other) noexcept;

  ~Epoll() = default;

  void Mod(int fd, epoll_event *event);
  void Add(int fd, uint32_t events);
  void Del(int fd);


  void SubscribeConnection(tcp::Connection *con_it);

  int Wait(std::vector<::epoll_event>& event_queue, size_t timeout);
private:
  desc::Descriptor epoll_fd_;

  void LogErrorAndThrowIf(bool cond);
};
}
