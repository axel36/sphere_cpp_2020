#include <iostream>

#include "logger.hpp"

namespace log {

void FileLogger::PrintLog(Level level, const std::string &msg) {
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
FileLogger::~FileLogger() { log_file_.flush(); }

void FileLogger::Flush() { log_file_.flush(); }

} // namespace log
