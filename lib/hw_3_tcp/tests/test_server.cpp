#include "logger.hpp"
#include "tcp.hpp"

int main() {
  log::init_with_stderr_logger(log::Level::DEBUG);

  tcp::Server server{"127.0.0.1", 8080, 100};
  server.SetTimeout(10, 0);
  tcp::Connection con = server.Accept();
  con.SetTimeout(5, 0);

  try {
    while (con.IsOpen()) {
      std::string buf(1111, '\0');
      size_t read = 0;
      read = con.Read(buf.data(), buf.size());
      std::cout << buf.substr(0, read) << std::flush;
    }
  } catch (const tcp::ConnectionError &) {
  }
  con = server.Accept();

  while (con.IsOpen()) {
    std::string buf(1111, '\0');
    size_t read = 0;
    read = con.Read(buf.data(), buf.size());
    std::cout << "con2 " << buf.substr(0, read) << std::flush;
  }
}
