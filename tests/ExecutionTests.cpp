#include "TestHarness.h"
#include "toygpu/WarpScheduler.h"

using namespace toygpu;

TEST(WarpExecAdd) {
  WarpState warp;
  std::vector<Instruction> prog;
  Instruction m1{Opcode::Mov, {{"%r1", PTXType::S32, false, 0}, {"1", PTXType::S32, true, 1}}};
  Instruction m2{Opcode::Mov, {{"%r2", PTXType::S32, false, 0}, {"2", PTXType::S32, true, 2}}};
  Instruction a{Opcode::Add, {{"%r3", PTXType::S32, false, 0}, {"%r1", PTXType::S32, false, 0}, {"%r2", PTXType::S32, false, 0}}};
  prog = {m1, m2, a};
  WarpScheduler s;
  auto perf = s.run(prog, warp);
  ASSERT_TRUE(perf.issuedInstructions >= 3);
  ASSERT_EQ(warp.registers[0][3], 3);
}
