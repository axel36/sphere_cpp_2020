#include <memory>

#include "logger.hpp"

namespace log {

BaseLogger &LoggerSingleton::GetGlobalLogger() { return *global_logger_; }

void LoggerSingleton::SetGlobalLogger(std::unique_ptr<BaseLogger> &logger) {
  global_logger_ = std::move(logger);
}

LoggerSingleton &LoggerSingleton::GetInstance() {
  static LoggerSingleton theSingleInstance;
  return theSingleInstance;
}

void init_with_stdout_logger(Level level) {
  LoggerSingleton &singleton = LoggerSingleton::GetInstance();
  std::unique_ptr<BaseLogger> logger = std::make_unique<StdoutLogger>();
  logger->SetLevel(level);
  singleton.SetGlobalLogger(logger);
}

void init_with_stderr_logger(Level level) {
  LoggerSingleton &singleton = LoggerSingleton::GetInstance();
  std::unique_ptr<BaseLogger> logger = std::make_unique<StderrLogger>();
  logger->SetLevel(level);
  singleton.SetGlobalLogger(logger);
}

void init_with_file_logger(Level level, const std::string &file_path) {
  LoggerSingleton &singleton = LoggerSingleton::GetInstance();
  std::unique_ptr<BaseLogger> logger = std::make_unique<FileLogger>(file_path);
  logger->SetLevel(level);
  singleton.SetGlobalLogger(logger);
}

void DEBUG(const std::string &msg) {
  BaseLogger &logger = LoggerSingleton::GetInstance().GetGlobalLogger();
  logger.Debug(msg);
}

void INFO(const std::string &msg) {
  BaseLogger &logger = LoggerSingleton::GetInstance().GetGlobalLogger();
  logger.Info(msg);
}

void WARN(const std::string &msg) {
  BaseLogger &logger = LoggerSingleton::GetInstance().GetGlobalLogger();
  logger.Warn(msg);
}

void ERROR(const std::string &msg) {
  BaseLogger &logger = LoggerSingleton::GetInstance().GetGlobalLogger();
  logger.Error(msg);
}

} // namespace log
