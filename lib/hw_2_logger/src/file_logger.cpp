#include <chrono>
#include <cstring>
#include <iostream>

#include "logger.hpp"

namespace log {

void FileLogger::PrintLog(const Level &level, const std::string &msg) {
  if (!log_file_.is_open() || log_file_.bad()) {
    throw std::runtime_error("wrong file");
  }

  auto log_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  char *time_str = ctime(&log_time);

  if (time_str[strlen(time_str) - 1] == '\n') {
    time_str[strlen(time_str) - 1] = '\0';
  }

  log_file_ << time_str << " [" << LevelToString(level) << "] " << msg
            << std::endl;
}

FileLogger::FileLogger(const std::string &filename) {
  log_file_.open(filename, std::ofstream::out | std::ofstream::app);
  if (!log_file_.is_open() || log_file_.bad()) {
    throw std::runtime_error("wrong file");
  }
}
FileLogger::~FileLogger() { log_file_.close(); }
void FileLogger::Flush() {}

} // namespace log
