#include "toygpu/BranchUnit.h"

namespace toygpu {

void BranchUnit::handleBranch(const Instruction&, WarpState& warp, std::size_t targetPc, std::size_t fallthroughPc) {
  stats_.branches++;
  bool evenTake = false, oddTake = false;
  for (int lane = 0; lane < 32; ++lane) {
    if (!warp.activeMask[lane]) continue;
    if (lane % 2 == 0) evenTake = true;
    else oddTake = true;
  }
  if (evenTake && oddTake) {
    stats_.divergentBranches++;
    ReconvergencePoint rp{fallthroughPc, warp.activeMask};
    warp.divergenceStack.push_back(rp);
    for (int lane = 0; lane < 32; ++lane) warp.activeMask[lane] = (lane % 2 == 0);
    warp.pc = targetPc;
  } else {
    warp.pc = targetPc;
  }
}

} // namespace toygpu
