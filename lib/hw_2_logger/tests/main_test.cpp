#include <memory>

#include "logger.hpp"

int main() {
  const auto logger2 = std::make_unique<log::FileLogger>("test.txt");
  logger2->Debug("asd");
  logger2->Info("info");
  logger2->Warn("warn");
  logger2->Error("error");

  const auto logger1 = std::make_unique<log::StderrLogger>();
  logger1->Debug("asd");
  logger1->Info("info");
  logger1->Warn("warn");
  logger1->Error("error");

  logger1->SetLevel(log::Level::ERROR);
  logger1->Info("info2");
  logger1->Warn("warn2");
  logger1->Error("error2");

  const auto logger3 = std::make_unique<log::StdoutLogger>();
  logger3->Debug("asd");
  logger3->Info("info");
  logger3->Warn("warn");
  logger3->Error("error");

  return 0;
}
