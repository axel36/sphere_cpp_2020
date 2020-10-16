#include <chrono>
#include <cstring>
#include <iostream>

#include "logger.hpp"

namespace log {

void StdoutLogger::PrintLog(const Level &level, const std::string &msg) {
  PrintLogInternal(std::cout, level, msg);
}
void StdoutLogger::Flush() {}

} // namespace log
