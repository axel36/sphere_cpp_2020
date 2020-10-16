#include <chrono>
#include <cstring>
#include <iostream>

#include "logger.hpp"

namespace log {

void StderrLogger::PrintLog(const Level &level, const std::string &msg) {
  PrintLogInternal(std::cerr, level, msg);
}

void StderrLogger::Flush() {}
} // namespace log
