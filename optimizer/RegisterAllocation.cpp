#include "toygpu/Pass.h"
#include <set>
#include <sstream>
#include <unordered_map>

namespace toygpu {

bool RegisterAllocationPass::run(Function& function) {
  std::unordered_map<std::string, std::set<std::string>> graph;
  std::set<std::string> live;
  for (auto bbIt = function.blocks.rbegin(); bbIt != function.blocks.rend(); ++bbIt) {
    for (auto it = bbIt->instructions.rbegin(); it != bbIt->instructions.rend(); ++it) {
      if (!it->operands.empty()) {
        std::string def = it->operands[0].name;
        for (const auto& l : live) {
          graph[def].insert(l);
          graph[l].insert(def);
        }
        live.erase(def);
      }
      for (std::size_t i = 1; i < it->operands.size(); ++i) live.insert(it->operands[i].name);
    }
  }

  std::unordered_map<std::string, int> color;
  constexpr int kRegs = 32;
  for (const auto& [node, neighbors] : graph) {
    std::set<int> used;
    for (const auto& n : neighbors) if (color.count(n)) used.insert(color[n]);
    int c = 0;
    while (used.count(c) && c < kRegs) ++c;
    color[node] = c < kRegs ? c : (kRegs + 1);
  }
  return !color.empty();
}

std::string RegisterAllocationPass::interferenceGraphDot(const Function& function) const {
  std::ostringstream os;
  os << "graph Interference {\n";
  for (const auto& [reg, _] : function.registers) os << "  \"" << reg << "\";\n";
  os << "}\n";
  return os.str();
}

} // namespace toygpu
