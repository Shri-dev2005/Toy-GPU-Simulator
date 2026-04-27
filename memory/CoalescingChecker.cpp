#include "toygpu/Memory.h"
#include <unordered_set>

namespace toygpu {

CoalescingReport analyzeCoalescing(const std::vector<std::size_t>& addresses, std::size_t lineSize) {
  std::unordered_set<std::size_t> lines;
  for (auto a : addresses) lines.insert(a / lineSize);
  CoalescingReport report;
  report.transactions = lines.size();
  report.coalesced = lines.size() <= 1;
  return report;
}

} // namespace toygpu
