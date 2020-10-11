#include <zconf.h>
#include <iostream>
#include "descriptor.hpp"

namespace desc {

Descriptor::Descriptor(int fd): fd_(fd) {
  if (!isValid()){
    throw DescriptorError("invald file descriptor");
  }
}

Descriptor::Descriptor(Descriptor &&other) noexcept: fd_(other.fd_) {
  other.fd_ = -1;
}

Descriptor &Descriptor::operator=(Descriptor &&other) noexcept {

  if (this != &other)
  {
    if(isValid()){
      ::close(fd_);
    }

    fd_ = other.fd_;
    other.fd_ = -1;
  }

  return *this;
}

Descriptor::~Descriptor() noexcept {
  try{
    close();
  }catch (const DescriptorError&){}
  catch(const std::exception& exc){
    std::cerr << exc.what() << std::endl;
  }
}

void Descriptor::close() {
  if(isValid()){
    ::close(fd_);
    fd_ = -1;
  } else {
    throw DescriptorError("can't close invalid descriptor");
  }

}

bool Descriptor::isValid() const{
  return fd_ > 0;
}

}
