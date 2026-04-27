#include "toygpu/WarpScheduler.h"

namespace toygpu {

PerfStats WarpScheduler::run(const std::vector<Instruction>& program, WarpState& warp) {
  PerfStats stats;
  while (warp.pc < program.size() && stats.cycles < 100000) {
    const auto& inst = program[warp.pc];
    if (inst.opcode == Opcode::Ld || inst.opcode == Opcode::St) {
      stats.stallCycles += 20;
      stats.cycles += 20;
    }
    if (inst.opcode == Opcode::Bra) {
      branchUnit_.handleBranch(inst, warp, warp.pc + 1, warp.pc + 1);
    } else {
      alu_.execute(inst, warp);
      warp.pc++;
    }
    stats.issuedInstructions++;
    stats.cycles++;

    if (warp.pc >= program.size() && !warp.divergenceStack.empty()) {
      auto rp = warp.divergenceStack.back();
      warp.divergenceStack.pop_back();
      warp.activeMask = rp.mask;
      warp.pc = rp.pc;
    }
  }
  return stats;
}

} // namespace toygpu
