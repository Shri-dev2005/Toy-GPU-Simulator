#include "toygpu/Memory.h"

namespace toygpu {

CacheModel::CacheModel(std::size_t sizeBytes, std::size_t lineSize, std::size_t ways)
    : lineSize_(lineSize), ways_(ways) {
  sets_ = sizeBytes / (lineSize_ * ways_);
  setLines_.assign(sets_, std::vector<Line>(ways_));
}

bool CacheModel::access(std::size_t address) {
  std::size_t lineAddr = address / lineSize_;
  std::size_t set = lineAddr % sets_;
  std::size_t tag = lineAddr / sets_;
  ++tick_;
  for (auto& line : setLines_[set]) {
    if (line.valid && line.tag == tag) {
      line.lru = tick_;
      stats_.hits++;
      return true;
    }
  }
  stats_.misses++;
  auto* victim = &setLines_[set][0];
  for (auto& line : setLines_[set]) {
    if (!line.valid || line.lru < victim->lru) victim = &line;
  }
  victim->valid = true;
  victim->tag = tag;
  victim->lru = tick_;
  return false;
}

} // namespace toygpu
