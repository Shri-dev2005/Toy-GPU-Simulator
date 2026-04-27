#include "toygpu/ALU.h"

namespace toygpu {

static int regIndex(const std::string& n) {
  if (n.size() < 3) return 0;
  return std::stoi(n.substr(2));
}

void ALU::execute(const Instruction& inst, WarpState& warp) {
  if (inst.operands.empty()) return;
  int dst = regIndex(inst.operands[0].name);
  for (int lane = 0; lane < 32; ++lane) {
    if (!warp.activeMask[lane]) continue;
    auto readOp = [&](std::size_t idx) -> int {
      if (idx >= inst.operands.size()) return 0;
      if (inst.operands[idx].isConstant) return static_cast<int>(inst.operands[idx].constValue);
      return warp.registers[lane][regIndex(inst.operands[idx].name)];
    };
    switch (inst.opcode) {
      case Opcode::Mov: warp.registers[lane][dst] = readOp(1); break;
      case Opcode::Add: warp.registers[lane][dst] = readOp(1) + readOp(2); break;
      case Opcode::Mul: warp.registers[lane][dst] = readOp(1) * readOp(2); break;
      case Opcode::Sub: warp.registers[lane][dst] = readOp(1) - readOp(2); break;
      default: break;
    }
  }
  warp.completedInstructions++;
}

} // namespace toygpu
