#pragma once

#include <memory>
#include "descriptor.hpp"
#include "epoll.hpp"

namespace net {

class IServiceListener{
public:
  virtual void OnNewConnection(tcp::Connection&) = 0;
  virtual void OnClose(tcp::Connection&) = 0;
  virtual void OnReadAvailable(tcp::Connection&) = 0;
  virtual void OnWriteDone(tcp::Connection&) = 0;
  virtual void OnError(tcp::Connection&) = 0;
};

class Service{
public:
  Service(const IServiceListener &listener);
  Service(size_t connections_max_count, std::unique_ptr<IServiceListener>&& listener);
  void Run();
private:
  Epoll epoll_;
  tcp::Server server_;
  std::unique_ptr<IServiceListener> listener_;
  size_t connections_max_count_;

  void OnEpollOut(tcp::Connection &con);
};

}
