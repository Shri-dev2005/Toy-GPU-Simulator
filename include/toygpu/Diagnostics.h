#pragma once
#include <sstream>
#include <string>
#include <vector>
#include "toygpu/SemanticValidator.h"

namespace toygpu {

class DiagnosticsEngine {
 public:
  void add(const Diagnostic& diagnostic);
  bool hasErrors() const;
  std::string render() const;

 private:
  std::vector<Diagnostic> diagnostics_;
};

} // namespace toygpu
