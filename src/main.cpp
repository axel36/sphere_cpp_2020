#include <chrono>
#include <iostream>

int main() {

  auto timenow =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::cout << ctime(&timenow) << std::endl;

  return 0;
}
