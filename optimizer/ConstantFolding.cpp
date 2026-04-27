#include "toygpu/Pass.h"

namespace toygpu {

bool ConstantFoldingPass::run(Function& function) {
  bool changed = false;
  for (auto& bb : function.blocks) {
    for (auto& inst : bb.instructions) {
      if ((inst.opcode == Opcode::Add || inst.opcode == Opcode::Mul || inst.opcode == Opcode::Sub) && inst.operands.size() >= 3 && inst.operands[1].isConstant && inst.operands[2].isConstant) {
        double a = inst.operands[1].constValue;
        double b = inst.operands[2].constValue;
        double out = inst.opcode == Opcode::Add ? a + b : inst.opcode == Opcode::Mul ? a * b : a - b;
        inst.opcode = Opcode::Mov;
        inst.operands.resize(2);
        inst.operands[1].name = std::to_string(out);
        inst.operands[1].isConstant = true;
        inst.operands[1].constValue = out;
        changed = true;
      }
    }
  }
  return changed;
}

} // namespace toygpu
