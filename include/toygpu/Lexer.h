#pragma once
#include <string>
#include <vector>

namespace toygpu {

enum class TokenKind {
  Identifier, Directive, Register, Number, Comma, Semicolon, Colon,
  LParen, RParen, At, End
};

struct Token {
  TokenKind kind;
  std::string text;
  int line;
};

class Lexer {
 public:
  explicit Lexer(std::string source);
  std::vector<Token> tokenize();

 private:
  std::string source_;
  std::size_t pos_{0};
  int line_{1};
  char peek() const;
  char get();
  void skipWhitespaceAndComments();
};

}  // namespace toygpu
