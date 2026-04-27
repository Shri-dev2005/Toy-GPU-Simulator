#pragma once
#include <cstddef>
#include "toygpu/IR.h"
#include "toygpu/WarpState.h"

namespace toygpu {

struct DivergenceStats {
  std::size_t branches{0};
  std::size_t divergentBranches{0};
};

class BranchUnit {
 public:
  void handleBranch(const Instruction& inst, WarpState& warp, std::size_t targetPc, std::size_t fallthroughPc);
  const DivergenceStats& stats() const { return stats_; }

 private:
  DivergenceStats stats_{};
};

} // namespace toygpu
