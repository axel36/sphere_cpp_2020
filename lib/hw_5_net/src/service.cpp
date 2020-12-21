#include <logger.hpp>
#include "net.hpp"

#include <memory>
#include <list>
#include <sys/epoll.h>
#include <algorithm>
#include <map>
#include <vector>

namespace net {

void Service::Run() {


  std::vector<::epoll_event> event_queue{1024};
  std::map<int, tcp::Connection> connections_;
  int events_count{};

  while(true) {
    try {
      events_count = epoll_.Wait(100);
    }catch(const EpollError& ex){
      // porcess error
      throw;
    }

    auto start = event_queue.begin();
    auto end = event_queue.begin() + events_count;
    for(auto it = start; it != end; ++it) {
      if(it->data.fd == server_.GetFD()) {

        auto con =  server_.Accept();
        log::INFO("get new client on " + con.GetInfo());

        listener_->OnNewConnection(con);
        epoll_.SubscribeConnection(&con);

        connections_.emplace(con.GetSocket(), std::move(con));

      } else if (it->events & EPOLLRDHUP) {
        auto client = std::move(connections_.at(it->data.fd));
        log::INFO("Disconnected client on " + client.GetInfo());

        listener_->OnClose(client);
        connections_.erase(it->data.fd);

      } else if (it->events & EPOLLIN) {
        auto& client = connections_.at(it->data.fd);
        listener_->OnReadAvailable(client);
        OnListenerEvent(client);
      } else if (it->events & EPOLLOUT){
        OnEpollOut(connections_.at(it->data.fd));
      } else if (it->events & EPOLLERR) {
        listener_->OnError(connections_.at(it->data.fd));
        connections_.erase(it->data.fd);
      }

    }
  }
}


void Service::OnEpollOut(tcp::Connection& con){

  con.WriteToConnection();
  if(con.write_buf_clear()){
    listener_->OnWriteDone(connections_.at(it->data.fd));
  };

}

Service::Service(size_t connections_max_count,
                 std::unique_ptr<IServiceListener> &&listener) : listener_(std::move(listener)), connections_max_count_(
    connections_max_count)  {}
}
