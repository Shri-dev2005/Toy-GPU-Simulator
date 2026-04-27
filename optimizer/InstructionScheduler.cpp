#include "toygpu/Pass.h"
#include <algorithm>

namespace toygpu {

bool InstructionSchedulerPass::run(Function& function) {
  bool changed = false;
  for (auto& bb : function.blocks) {
    std::stable_sort(bb.instructions.begin(), bb.instructions.end(), [](const Instruction& a, const Instruction& b) {
      return a.hasSideEffects() < b.hasSideEffects();
    });
    changed = changed || !bb.instructions.empty();
  }
  return changed;
}

} // namespace toygpu
