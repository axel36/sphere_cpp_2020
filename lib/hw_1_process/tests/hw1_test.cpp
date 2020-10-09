#include "Process.hpp"
#include <cassert>
#include <stdexcept>

int main() {

  proc::Process proc{"./test_1hw.sh"};

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

  proc.closeStdin();

  assert(proc.isChannelOpen() == false);
  try {
    proc.write(element.data(), 10);
    assert(false);
  } catch (const std::runtime_error &exc) {
    const std::string expected_what = "can't write message: channel was closed";
    assert(exc.what() == expected_what);
  }

  assert(proc.isProcessRunning() == true);
  proc.close();
  assert(proc.isProcessRunning() == false);

  try {
    std::string test(10, '\0');
    proc.write(test.data(), 10);
    assert(false);
  } catch (const std::runtime_error &exc) {
    const std::string expected_what = "can't write message: channel was closed";
    assert(exc.what() == expected_what);
  }

  return 0;
}
