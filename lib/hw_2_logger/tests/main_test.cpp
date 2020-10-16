#include "logger.hpp"
#include <iostream>
#include <memory>

int main() {
  const auto logger2 = std::make_unique<log::FileLogger>("test.txt");
  logger2->Debug("asd");
  logger2->Info("info");
  logger2->Warn("warn");
  logger2->Error("error");

  return 0;
}
