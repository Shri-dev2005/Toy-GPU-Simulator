#include "toygpu/Logger.h"
#include <iostream>

namespace toygpu {

static LogLevel g_level = LogLevel::Info;

void Logger::setLevel(LogLevel level) { g_level = level; }

void Logger::log(LogLevel level, const std::string& message) {
  if (static_cast<int>(level) > static_cast<int>(g_level)) return;
  std::cerr << "[" << static_cast<int>(level) << "] " << message << "\n";
}

} // namespace toygpu
