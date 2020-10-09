#include <Process.hpp>
#include <cassert>
#include <iostream>

int main() {

  proc::Process proc{"./write.sh"};

  std::string input_string;
  std::string element = "1234567890";
  int len = 1000;

  for (int i = 0; i < len; i++) {
    input_string += element;
  }
  std::string ended_string = input_string + "\n\0";

  proc.writeExact(ended_string.data(), element.size() * len + 2);

  std::string out(element.size() * len + 2, '\0');
  proc.readExact(out.data(), 10002);

  input_string += "_$";
  assert(input_string == out);

  return 0;
}
