#include "logger.hpp"
#include "tcp.hpp"

int main() {
  log::init_with_stderr_logger(log::Level::DEBUG);

  tcp::ClientConnection con{"127.0.0.1", 8080};

  while (con.IsOpen()) {
    std::string buf(1111, '\0');
    size_t read = 0;
    read = con.Read(buf.data(), buf.size());
    std::cout << buf.substr(0, read) << std::flush;
  }

}
