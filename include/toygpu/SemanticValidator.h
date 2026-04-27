#pragma once
#include <string>
#include <vector>
#include "toygpu/IR.h"

namespace toygpu {

struct Diagnostic {
  enum class Level { Error, Warning, Info };
  Level level;
  std::string message;
};

class SemanticValidator {
 public:
  std::vector<Diagnostic> validate(Module& module);
};

}  // namespace toygpu
