#include <Process.hpp>
#include <iostream>

int main() {

  try {
    proc::Process proc{"./write.sh"};


    proc.writeExact("very long string \n\0", 19);
    proc.writeExact("another long string2 \n\0", 23);

    std::string out(100, '\0');
    proc.readExact(out.data(), 25);
    std::cout << "1: " << out << std::endl;

    out = std::string(100, '\0');
    proc.readExact(out.data(), 10);
    std::cout << "2: " << out << std::endl;

  } catch (const std::runtime_error &e) {
    std::cout << "err: " << e.what() << std::endl;
  }
  return 0;
}
