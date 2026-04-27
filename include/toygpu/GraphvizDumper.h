#pragma once
#include <string>
#include "toygpu/IR.h"

namespace toygpu {

std::string dumpCFGDot(const Function& function);

} // namespace toygpu
