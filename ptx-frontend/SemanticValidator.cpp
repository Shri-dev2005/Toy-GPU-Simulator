#include "toygpu/SemanticValidator.h"
#include <unordered_set>

namespace toygpu {

std::vector<Diagnostic> SemanticValidator::validate(Module& module) {
  std::vector<Diagnostic> diags;
  for (auto& fn : module.functions) {
    std::unordered_set<std::string> labels;
    for (const auto& bb : fn.blocks) labels.insert(bb.label);
    for (const auto& bb : fn.blocks) {
      for (const auto& inst : bb.instructions) {
        if (!inst.predicate.empty() && fn.registers.find(inst.predicate) == fn.registers.end()) {
          diags.push_back({Diagnostic::Level::Error, "Undefined predicate register: " + inst.predicate});
        }
        for (const auto& op : inst.operands) {
          if (!op.isConstant && !op.name.empty() && op.name[0] == '%' && fn.registers.find(op.name) == fn.registers.end()) {
            diags.push_back({Diagnostic::Level::Error, "Undefined register: " + op.name});
          }
        }
        if (inst.opcode == Opcode::Bra && !inst.targetLabel.empty() && !labels.count(inst.targetLabel)) {
          diags.push_back({Diagnostic::Level::Error, "Invalid branch target: " + inst.targetLabel});
        }
      }
    }
  }
  return diags;
}

} // namespace toygpu
