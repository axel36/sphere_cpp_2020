#include <chrono>
#include <cstring>
#include <iostream>

#include "logger.hpp"

namespace log {

void StderrLogger::PrintLog(const Level &level, const std::string &msg) {
  auto log_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char *time_str = ctime(&log_time);

  if (time_str[strlen(time_str) - 1] == '\n')
    time_str[strlen(time_str) - 1] = '\0';

  std::cerr << time_str << " [" << LevelToString(level) << "] " << msg
            << std::endl;
}

void StderrLogger::Flush() {}
} // namespace log
