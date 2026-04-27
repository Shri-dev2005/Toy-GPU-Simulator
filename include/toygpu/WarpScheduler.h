#pragma once
#include <cstddef>
#include <vector>
#include "toygpu/ALU.h"
#include "toygpu/BranchUnit.h"
#include "toygpu/IR.h"
#include "toygpu/WarpState.h"

namespace toygpu {

struct PerfStats {
  std::size_t cycles{0};
  std::size_t issuedInstructions{0};
  std::size_t stallCycles{0};
  double ipc() const { return cycles ? static_cast<double>(issuedInstructions) / cycles : 0.0; }
};

class WarpScheduler {
 public:
  PerfStats run(const std::vector<Instruction>& program, WarpState& warp);
  const DivergenceStats& divergenceStats() const { return branchUnit_.stats(); }

 private:
  ALU alu_;
  BranchUnit branchUnit_;
};

} // namespace toygpu
