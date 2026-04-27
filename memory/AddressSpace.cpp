#include "toygpu/Memory.h"
#include <stdexcept>

namespace toygpu {

AddressSpace::AddressSpace(std::size_t g, std::size_t s, std::size_t c, std::size_t l)
    : global_(g, 0), shared_(s, 0), constant_(c, 0), local_(l, 0) {}

const std::vector<uint8_t>& AddressSpace::region(AddressSpaceKind kind) const {
  switch (kind) {
    case AddressSpaceKind::Global: return global_;
    case AddressSpaceKind::Shared: return shared_;
    case AddressSpaceKind::Constant: return constant_;
    case AddressSpaceKind::Local: return local_;
  }
  return global_;
}

std::vector<uint8_t>& AddressSpace::region(AddressSpaceKind kind) {
  return const_cast<std::vector<uint8_t>&>(static_cast<const AddressSpace*>(this)->region(kind));
}

uint32_t AddressSpace::load32(AddressSpaceKind kind, std::size_t address) const {
  const auto& r = region(kind);
  if (address + 4 > r.size()) throw std::out_of_range("load out of range");
  return r[address] | (r[address + 1] << 8) | (r[address + 2] << 16) | (r[address + 3] << 24);
}

void AddressSpace::store32(AddressSpaceKind kind, std::size_t address, uint32_t v) {
  auto& r = region(kind);
  if (address + 4 > r.size()) throw std::out_of_range("store out of range");
  r[address] = static_cast<uint8_t>(v & 0xFF);
  r[address + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
  r[address + 2] = static_cast<uint8_t>((v >> 16) & 0xFF);
  r[address + 3] = static_cast<uint8_t>((v >> 24) & 0xFF);
}

} // namespace toygpu
