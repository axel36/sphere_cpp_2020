#include "logger.hpp"
#include <chrono>
#include <cstring>
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

void BaseLogger::SetLevel(const Level &level) { level_ = level; }
Level BaseLogger::GetCurrentLevel() { return level_; }

void BaseLogger::Log(const Level &level, const std::string &msg) {
  if (GetCurrentLevel() <= level) {
    PrintLog(level, msg);
  }
}
void BaseLogger::Debug(const std::string &msg) { Log(Level::DEBUG, msg); }
void BaseLogger::Info(const std::string &msg) { Log(Level::INFO, msg); }
void BaseLogger::Warn(const std::string &msg) { Log(Level::WARN, msg); }
void BaseLogger::Error(const std::string &msg) { Log(Level::ERROR, msg); }

void BaseLogger::PrintLogInternal(std::ostream &stream, const Level &level,
                                  const std::string &msg) {
  auto log_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char *time_str = ctime(&log_time);
  if (time_str[strlen(time_str) - 1] == '\n') {
    time_str[strlen(time_str) - 1] = '\0';
  }

  stream << time_str << " [" << LevelToString(level) << "] " << msg
         << std::endl;
}

} // namespace log
