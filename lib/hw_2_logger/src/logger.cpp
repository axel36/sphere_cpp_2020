#include "logger.hpp"
#include <stdexcept>

namespace log {

std::string LevelToString(const Level &level) {
  switch (level) {
  case Level::DEBUG:
    return "DEBUG";
  case Level::INFO:
    return "INFO";
  case Level::WARN:
    return "WARN";
  case Level::ERROR:
    return "ERROR";
  }
  throw std::runtime_error("Unknown log level");
}

void BaseLogger::Log(const Level &level, const std::string &msg) {
  if (GetCurrentLevel() <= level) {
    PrintLog(level, msg);
  }
}

void BaseLogger::Debug(const std::string &msg) { Log(Level::DEBUG, msg); }
void BaseLogger::Info(const std::string &msg) { Log(Level::INFO, msg); }
void BaseLogger::Warn(const std::string &msg) { Log(Level::WARN, msg); }
void BaseLogger::Error(const std::string &msg) { Log(Level::ERROR, msg); }

} // namespace log
