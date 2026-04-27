#include "toygpu/IR.h"
#include <sstream>

namespace toygpu {

bool Instruction::hasSideEffects() const {
  return opcode == Opcode::St || opcode == Opcode::Call || opcode == Opcode::Bra || opcode == Opcode::Ret || opcode == Opcode::Barrier;
}

std::string opcodeToString(Opcode opcode) {
  switch (opcode) {
    case Opcode::Mov: return "mov";
    case Opcode::Add: return "add";
    case Opcode::Mul: return "mul";
    case Opcode::Sub: return "sub";
    case Opcode::Setp: return "setp";
    case Opcode::Cvt: return "cvt";
    case Opcode::Ld: return "ld";
    case Opcode::St: return "st";
    case Opcode::Bra: return "bra";
    case Opcode::Call: return "call";
    case Opcode::Ret: return "ret";
    case Opcode::Barrier: return "barrier";
    default: return "nop";
  }
}

std::string Instruction::toString() const {
  std::ostringstream os;
  if (!predicate.empty()) os << "@" << predicate << " ";
  os << opcodeToString(opcode);
  for (std::size_t i = 0; i < operands.size(); ++i) {
    os << (i == 0 ? " " : ", ") << operands[i].name;
  }
  if (!targetLabel.empty()) os << " -> " << targetLabel;
  return os.str();
}

} // namespace toygpu
