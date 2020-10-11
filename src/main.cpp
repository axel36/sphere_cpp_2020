#include <iostream>
#include "descriptor.hpp"

int main() {
  desc::Descriptor x{};
  try {
    x.close();
  } catch (const desc::DescriptorError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
