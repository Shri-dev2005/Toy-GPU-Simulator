#include "toygpu/Lexer.h"
#include <cctype>

namespace toygpu {

Lexer::Lexer(std::string source) : source_(std::move(source)) {}
char Lexer::peek() const { return pos_ < source_.size() ? source_[pos_] : '\0'; }
char Lexer::get() { return pos_ < source_.size() ? source_[pos_++] : '\0'; }

void Lexer::skipWhitespaceAndComments() {
  while (true) {
    while (std::isspace(static_cast<unsigned char>(peek()))) {
      if (peek() == '\n') ++line_;
      get();
    }
    if (peek() == '/' && pos_ + 1 < source_.size() && source_[pos_ + 1] == '/') {
      while (peek() && peek() != '\n') get();
      continue;
    }
    break;
  }
}

std::vector<Token> Lexer::tokenize() {
  std::vector<Token> tokens;
  while (true) {
    skipWhitespaceAndComments();
    char c = peek();
    if (!c) break;
    if (std::isalpha(static_cast<unsigned char>(c)) || c == '.' || c == '_') {
      std::string s;
      while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '.' || peek() == '_' ) s.push_back(get());
      TokenKind k = s[0] == '.' ? TokenKind::Directive : TokenKind::Identifier;
      tokens.push_back({k, s, line_});
    } else if (c == '%') {
      std::string s;
      s.push_back(get());
      while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') s.push_back(get());
      tokens.push_back({TokenKind::Register, s, line_});
    } else if (std::isdigit(static_cast<unsigned char>(c)) || c == '-') {
      std::string s;
      s.push_back(get());
      while (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '.') s.push_back(get());
      tokens.push_back({TokenKind::Number, s, line_});
    } else {
      TokenKind k = TokenKind::End;
      switch (get()) {
        case ',': k = TokenKind::Comma; break;
        case ';': k = TokenKind::Semicolon; break;
        case ':': k = TokenKind::Colon; break;
        case '(': k = TokenKind::LParen; break;
        case ')': k = TokenKind::RParen; break;
        case '@': k = TokenKind::At; break;
        default: continue;
      }
      tokens.push_back({k, "", line_});
    }
  }
  tokens.push_back({TokenKind::End, "", line_});
  return tokens;
}

} // namespace toygpu
