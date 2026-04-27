#include "TestHarness.h"
#include "toygpu/Memory.h"

using namespace toygpu;

TEST(AddressSpaceRoundTrip) {
  AddressSpace mem(1024, 1024, 256, 256);
  mem.store32(AddressSpaceKind::Global, 8, 0xDEADBEEF);
  ASSERT_EQ(mem.load32(AddressSpaceKind::Global, 8), 0xDEADBEEF);
}

TEST(CacheHitMiss) {
  CacheModel l1(32 * 1024, 128, 4);
  bool h1 = l1.access(0x1000);
  bool h2 = l1.access(0x1000);
  ASSERT_TRUE(!h1 && h2);
}
