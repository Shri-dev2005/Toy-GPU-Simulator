#pragma once
#include <string>
#include <vector>
#include "toygpu/IR.h"
#include "toygpu/Lexer.h"

namespace toygpu {

class Parser {
 public:
  explicit Parser(std::vector<Token> tokens);
  Module parseModule();

 private:
  std::vector<Token> tokens_;
  std::size_t idx_{0};
  Token& current();
  bool match(TokenKind kind, const std::string& text = "");
  bool consume(TokenKind kind, const std::string& text = "");
  Function parseFunction();
  BasicBlock parseBlock();
  Instruction parseInstruction();
  void buildCFG(Function& fn);
};

}  // namespace toygpu
