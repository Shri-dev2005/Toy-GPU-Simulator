#include "TestHarness.h"
#include "toygpu/Lexer.h"
#include "toygpu/Parser.h"

using namespace toygpu;

TEST(FrontendParseBasic) {
  std::string src = ".entry k\n.reg .s32 %r1;\nentry:\n mov %r1, 1;\n ret;";
  Lexer lx(src);
  Parser p(lx.tokenize());
  auto m = p.parseModule();
  ASSERT_EQ(m.functions.size(), 1u);
  ASSERT_EQ(m.functions[0].blocks.size(), 1u);
}
