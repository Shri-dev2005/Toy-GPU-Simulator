#pragma once
#include "toygpu/IR.h"
#include "toygpu/WarpState.h"

namespace toygpu {

class ALU {
 public:
  void execute(const Instruction& inst, WarpState& warp);
};

} // namespace toygpu
