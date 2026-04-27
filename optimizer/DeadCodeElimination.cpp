#include "toygpu/Pass.h"
#include <unordered_set>

namespace toygpu {

bool DeadCodeEliminationPass::run(Function& function) {
  bool changed = false;
  std::unordered_set<std::string> live;
  for (auto bbIt = function.blocks.rbegin(); bbIt != function.blocks.rend(); ++bbIt) {
    auto& insts = bbIt->instructions;
    for (auto it = insts.rbegin(); it != insts.rend();) {
      bool keep = it->hasSideEffects();
      if (!it->operands.empty()) {
        const auto& def = it->operands[0].name;
        if (live.count(def)) keep = true;
      }
      for (std::size_t i = 1; i < it->operands.size(); ++i) live.insert(it->operands[i].name);
      if (!keep) {
        it = decltype(it)(insts.erase(std::next(it).base()));
        changed = true;
      } else {
        ++it;
      }
    }
  }
  return changed;
}

} // namespace toygpu
