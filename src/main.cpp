#include <iostream>
#include <Process.hpp>

int main() {
  proc::Process proc{"date"};

  std::string out(1024, '\0');
  proc.readExact(out.data(), 10);
  proc.readExact(out.data()+10, 10);
  std::cout << out << std::endl;

  return 0;
}
