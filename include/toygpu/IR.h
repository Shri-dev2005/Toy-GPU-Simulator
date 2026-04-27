#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "toygpu/TypeSystem.h"

namespace toygpu {

enum class Opcode {
  Nop, Mov, Add, Mul, Sub, Setp, Cvt, Ld, St, Bra, Call, Ret, Barrier
};

struct Value {
  std::string name;
  PTXType type{PTXType::Invalid};
  bool isConstant{false};
  double constValue{0.0};
};

struct Instruction {
  Opcode opcode{Opcode::Nop};
  std::vector<Value> operands;
  std::string predicate;
  std::string targetLabel;
  bool hasSideEffects() const;
  std::string toString() const;
};

struct BasicBlock {
  std::string label;
  std::vector<Instruction> instructions;
  std::vector<std::string> predecessors;
  std::vector<std::string> successors;
};

struct Function {
  std::string name;
  bool isEntry{false};
  std::vector<BasicBlock> blocks;
  std::unordered_map<std::string, PTXType> registers;
};

struct Module {
  std::vector<Function> functions;
};

std::string opcodeToString(Opcode opcode);

}  // namespace toygpu
