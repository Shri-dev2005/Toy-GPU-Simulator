#pragma once
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace toygpu {

enum class AddressSpaceKind { Global, Shared, Constant, Local };

class AddressSpace {
 public:
  AddressSpace(std::size_t globalSize, std::size_t sharedSize, std::size_t constantSize, std::size_t localSize);
  uint32_t load32(AddressSpaceKind kind, std::size_t address) const;
  void store32(AddressSpaceKind kind, std::size_t address, uint32_t value);

 private:
  std::vector<uint8_t> global_, shared_, constant_, local_;
  const std::vector<uint8_t>& region(AddressSpaceKind kind) const;
  std::vector<uint8_t>& region(AddressSpaceKind kind);
};

struct CacheStats { std::size_t hits{0}; std::size_t misses{0}; };

class CacheModel {
 public:
  CacheModel(std::size_t sizeBytes, std::size_t lineSize, std::size_t ways);
  bool access(std::size_t address);
  const CacheStats& stats() const { return stats_; }

 private:
  struct Line { bool valid{false}; std::size_t tag{0}; std::size_t lru{0}; };
  std::size_t lineSize_, ways_, sets_, tick_{0};
  std::vector<std::vector<Line>> setLines_;
  CacheStats stats_;
};

struct CoalescingReport { bool coalesced{false}; std::size_t transactions{0}; };
CoalescingReport analyzeCoalescing(const std::vector<std::size_t>& addresses, std::size_t lineSize = 128);

struct BankConflictReport { std::size_t maxConflict{0}; std::size_t serializedAccesses{0}; };
BankConflictReport analyzeBankConflicts(const std::vector<std::size_t>& sharedAddresses, std::size_t bankWidth = 4);

} // namespace toygpu
