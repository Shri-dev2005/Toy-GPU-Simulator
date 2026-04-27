#include "TestHarness.h"
#include "toygpu/Pass.h"

using namespace toygpu;

TEST(ConstantFoldingWorks) {
  Function fn;
  BasicBlock bb; bb.label = "entry";
  Instruction inst;
  inst.opcode = Opcode::Add;
  inst.operands = {{"%r1", PTXType::S32, false, 0}, {"2", PTXType::S32, true, 2}, {"3", PTXType::S32, true, 3}};
  bb.instructions.push_back(inst);
  fn.blocks.push_back(bb);
  ConstantFoldingPass pass;
  ASSERT_TRUE(pass.run(fn));
  ASSERT_EQ(fn.blocks[0].instructions[0].opcode, Opcode::Mov);
}
