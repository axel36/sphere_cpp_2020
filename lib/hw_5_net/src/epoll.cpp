#include <cstring>

#include <sys/epoll.h>
#include <vector>

#include "net.hpp"

#include "logger.hpp"

namespace net{
Epoll::Epoll() {
  try {
    epoll_fd_ = desc::Descriptor{::epoll_create(1)};
  } catch(const desc::DescriptorError&){
    log::ERROR(std::strerror(errno));
    throw EpollError(std::strerror(errno));
  }

}
Epoll::Epoll(Epoll &&other) noexcept: epoll_fd_(std::move(other.epoll_fd_)) {}

Epoll &Epoll::operator=(Epoll &&other) noexcept {
  if (this != &other) {
    epoll_fd_ = std::move(other.epoll_fd_);
  }
  return *this;
}

void Epoll::Mod(int fd, ::epoll_event* event) {
  int result = ::epoll_ctl(*epoll_fd_, EPOLL_CTL_MOD, fd, event);
  LogErrorAndThrowIf(result < 0);

  log::DEBUG("fd " + std::to_string(fd) + "was modified in epoll " + std::to_string(*epoll_fd_));
}

void Epoll::Add(int fd, uint32_t event_events) {
  ::epoll_event event{};
  event.events = event_events;
  event.data.fd = fd;

  int result = ::epoll_ctl(*epoll_fd_, EPOLL_CTL_ADD, fd, &event);
  LogErrorAndThrowIf(result < 0);
  log::DEBUG("fd " + std::to_string(fd) + "was added to epoll " + std::to_string(*epoll_fd_));
}

void Epoll::Del(int fd) {
  ::epoll_event event{};
  int result = ::epoll_ctl(*epoll_fd_, EPOLL_CTL_DEL, fd, &event);
  LogErrorAndThrowIf(result < 0);
  log::DEBUG("fd " + std::to_string(fd) + "was deleted from epoll " + std::to_string(*epoll_fd_));

}

int Epoll::Wait(std::vector<::epoll_event>& event_queue, size_t timeout) {

  int events_count = ::epoll_wait(*epoll_fd_, event_queue.data(),
                                  event_queue.size(), -1);

  if (events_count < 0){
    throw EpollError("Error on waiting event");
  }

  return events_count;
}

void Epoll::LogErrorAndThrowIf(bool cond) {
    log::ERROR(std::strerror(errno));
    throw EpollError(std::strerror(errno));
}

void Epoll::SubscribeConnection(tcp::Connection* con_it) {
  ::epoll_event event{};
  event.events = EPOLLIN | EPOLLRDHUP;
  event.data.fd = con_it->GetSocket();
  event.data.ptr = con_it;

  int result = ::epoll_ctl(*epoll_fd_, EPOLL_CTL_ADD, con_it->GetSocket(), &event);
  LogErrorAndThrowIf(result < 0);
}

}
