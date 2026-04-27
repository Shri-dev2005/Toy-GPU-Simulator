#include "toygpu/Pass.h"

namespace toygpu {

void PassManager::addFunctionPass(std::unique_ptr<FunctionPass> pass) { functionPasses_.push_back(std::move(pass)); }
void PassManager::addModulePass(std::unique_ptr<ModulePass> pass) { modulePasses_.push_back(std::move(pass)); }

void PassManager::run(Module& module) {
  for (auto& pass : modulePasses_) pass->run(module);
  for (auto& fn : module.functions) {
    for (auto& pass : functionPasses_) pass->run(fn);
  }
}

} // namespace toygpu
