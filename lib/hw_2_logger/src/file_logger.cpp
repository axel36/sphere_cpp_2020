#include <chrono>
#include <cstring>
#include <iostream>

#include "logger.hpp"

namespace log {

void FileLogger::PrintLog(const Level &level, const std::string &msg) {
  if (!log_file_.is_open() || log_file_.bad()) {
    throw std::runtime_error("wrong file");
  }
  PrintLogInternal(log_file_, level, msg);
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
