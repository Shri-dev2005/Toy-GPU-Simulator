#pragma once
#include <memory>
#include <vector>
#include "toygpu/IR.h"

namespace toygpu {

class FunctionPass {
 public:
  virtual ~FunctionPass() = default;
  virtual bool run(Function& function) = 0;
};

class ModulePass {
 public:
  virtual ~ModulePass() = default;
  virtual bool run(Module& module) = 0;
};

class PassManager {
 public:
  void addFunctionPass(std::unique_ptr<FunctionPass> pass);
  void addModulePass(std::unique_ptr<ModulePass> pass);
  void run(Module& module);

 private:
  std::vector<std::unique_ptr<FunctionPass>> functionPasses_;
  std::vector<std::unique_ptr<ModulePass>> modulePasses_;
};

class DeadCodeEliminationPass : public FunctionPass {
 public:
  bool run(Function& function) override;
};

class ConstantFoldingPass : public FunctionPass {
 public:
  bool run(Function& function) override;
};

class RegisterAllocationPass : public FunctionPass {
 public:
  bool run(Function& function) override;
  std::string interferenceGraphDot(const Function& function) const;
};

class InstructionSchedulerPass : public FunctionPass {
 public:
  bool run(Function& function) override;
};

}  // namespace toygpu
