#include "toygpu/GraphvizDumper.h"
#include <sstream>

namespace toygpu {

std::string dumpCFGDot(const Function& function) {
  std::ostringstream os;
  os << "digraph CFG_" << function.name << " {\n";
  for (const auto& bb : function.blocks) {
    os << "  \"" << bb.label << "\";\n";
    for (const auto& succ : bb.successors) {
      os << "  \"" << bb.label << "\" -> \"" << succ << "\";\n";
    }
  }
  os << "}\n";
  return os.str();
}

} // namespace toygpu
