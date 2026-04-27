#include "toygpu/Memory.h"
#include <array>

namespace toygpu {

BankConflictReport analyzeBankConflicts(const std::vector<std::size_t>& sharedAddresses, std::size_t bankWidth) {
  std::array<std::size_t, 32> bankCounts{};
  for (auto address : sharedAddresses) {
    std::size_t bank = (address / bankWidth) % 32;
    bankCounts[bank]++;
  }
  BankConflictReport report;
  for (auto c : bankCounts) {
    if (c > report.maxConflict) report.maxConflict = c;
    if (c > 1) report.serializedAccesses += c - 1;
  }
  return report;
}

} // namespace toygpu
