#pragma once
#include <string>

namespace toygpu {

enum class LogLevel { Error, Warn, Info, Debug };

class Logger {
 public:
  static void setLevel(LogLevel level);
  static void log(LogLevel level, const std::string& message);
};

} // namespace toygpu
