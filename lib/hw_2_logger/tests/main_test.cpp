#include <cassert>
#include <memory>

#include "logger.hpp"
#include "test_class.hpp"

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

  log::init_with_stdout_logger(log::Level::WARN);

  log::DEBUG("global_Debug");
  log::INFO("global_Info");
  log::WARN("global_Warn");
  log::ERROR("global_Error");

  log::init_with_stderr_logger(log::Level::DEBUG);

  log::DEBUG("another_global_Debug");
  log::INFO("another_global_Info");
  log::WARN("another_global_Warn");
  log::ERROR("another_global_Error");

  {
    test_class test{};
    log::DEBUG("before class death");
  }

  try {
    auto &logger = log::LoggerSingleton::GetInstance();
    logger.SetGlobalLogger({});
    assert(false);
  } catch (const log::LoggerError &exc) {
    std::string expected_what = "try to init logger with nullptr";
    assert(exc.what() == expected_what);
  }

  return 0;
}
