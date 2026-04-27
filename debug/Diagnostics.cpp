#include "toygpu/Diagnostics.h"

namespace toygpu {

void DiagnosticsEngine::add(const Diagnostic& diagnostic) { diagnostics_.push_back(diagnostic); }
bool DiagnosticsEngine::hasErrors() const {
  for (const auto& d : diagnostics_) if (d.level == Diagnostic::Level::Error) return true;
  return false;
}

std::string DiagnosticsEngine::render() const {
  std::ostringstream os;
  for (const auto& d : diagnostics_) {
    os << (d.level == Diagnostic::Level::Error ? "error: " : d.level == Diagnostic::Level::Warning ? "warning: " : "info: ")
       << d.message << '\n';
  }
  return os.str();
}

} // namespace toygpu
