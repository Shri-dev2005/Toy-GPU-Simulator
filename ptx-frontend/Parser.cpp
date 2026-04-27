#include "toygpu/Parser.h"
#include <stdexcept>
#include <unordered_map>

namespace toygpu {

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}
Token& Parser::current() { return tokens_[idx_]; }
bool Parser::match(TokenKind kind, const std::string& text) { return current().kind == kind && (text.empty() || current().text == text); }
bool Parser::consume(TokenKind kind, const std::string& text) {
  if (!match(kind, text)) return false;
  ++idx_;
  return true;
}

static Opcode parseOpcode(const std::string& op) {
  if (op == "mov") return Opcode::Mov;
  if (op == "add") return Opcode::Add;
  if (op == "mul") return Opcode::Mul;
  if (op == "sub") return Opcode::Sub;
  if (op == "setp") return Opcode::Setp;
  if (op == "cvt") return Opcode::Cvt;
  if (op == "ld") return Opcode::Ld;
  if (op == "st") return Opcode::St;
  if (op == "bra") return Opcode::Bra;
  if (op == "ret") return Opcode::Ret;
  if (op == "barrier") return Opcode::Barrier;
  return Opcode::Nop;
}

Instruction Parser::parseInstruction() {
  Instruction inst;
  if (consume(TokenKind::At)) {
    if (!match(TokenKind::Register)) throw std::runtime_error("expected predicate register");
    inst.predicate = current().text;
    ++idx_;
  }
  if (!match(TokenKind::Identifier)) throw std::runtime_error("expected opcode");
  inst.opcode = parseOpcode(current().text);
  ++idx_;

  while (!match(TokenKind::Semicolon) && !match(TokenKind::End)) {
    if (match(TokenKind::Register) || match(TokenKind::Identifier) || match(TokenKind::Number)) {
      Value v;
      v.name = current().text;
      v.isConstant = match(TokenKind::Number);
      if (v.isConstant) v.constValue = std::stod(v.name);
      inst.operands.push_back(v);
      ++idx_;
    } else {
      ++idx_;
    }
  }
  consume(TokenKind::Semicolon);
  if (inst.opcode == Opcode::Bra && !inst.operands.empty()) {
    inst.targetLabel = inst.operands.back().name;
  }
  return inst;
}

BasicBlock Parser::parseBlock() {
  BasicBlock bb;
  if (!match(TokenKind::Identifier)) throw std::runtime_error("expected label");
  bb.label = current().text;
  ++idx_;
  consume(TokenKind::Colon);
  while (!match(TokenKind::End) && !(match(TokenKind::Identifier) && tokens_[idx_ + 1].kind == TokenKind::Colon)) {
    bb.instructions.push_back(parseInstruction());
  }
  return bb;
}

Function Parser::parseFunction() {
  Function fn;
  consume(TokenKind::Directive, ".entry") || consume(TokenKind::Directive, ".func");
  fn.isEntry = tokens_[idx_ - 1].text == ".entry";
  if (!match(TokenKind::Identifier)) throw std::runtime_error("expected function name");
  fn.name = current().text;
  ++idx_;
  while (!match(TokenKind::End) && !match(TokenKind::Directive, ".entry") && !match(TokenKind::Directive, ".func")) {
    if (match(TokenKind::Directive, ".reg")) {
      ++idx_;
      auto t = parseTypeToken(current().text);
      ++idx_;
      if (match(TokenKind::Register)) {
        fn.registers[current().text] = t.value_or(PTXType::Invalid);
        ++idx_;
      }
      consume(TokenKind::Semicolon);
      continue;
    }
    if (match(TokenKind::Identifier) && tokens_[idx_ + 1].kind == TokenKind::Colon) {
      fn.blocks.push_back(parseBlock());
    } else {
      ++idx_;
    }
  }
  buildCFG(fn);
  return fn;
}

void Parser::buildCFG(Function& fn) {
  std::unordered_map<std::string, std::size_t> blockIndex;
  for (std::size_t i = 0; i < fn.blocks.size(); ++i) blockIndex[fn.blocks[i].label] = i;
  for (std::size_t i = 0; i < fn.blocks.size(); ++i) {
    auto& bb = fn.blocks[i];
    if (bb.instructions.empty()) continue;
    const auto& term = bb.instructions.back();
    if (term.opcode == Opcode::Bra && blockIndex.count(term.targetLabel)) {
      bb.successors.push_back(term.targetLabel);
      fn.blocks[blockIndex[term.targetLabel]].predecessors.push_back(bb.label);
    } else if (term.opcode != Opcode::Ret && i + 1 < fn.blocks.size()) {
      bb.successors.push_back(fn.blocks[i + 1].label);
      fn.blocks[i + 1].predecessors.push_back(bb.label);
    }
  }
}

Module Parser::parseModule() {
  Module m;
  while (!match(TokenKind::End)) {
    if (match(TokenKind::Directive, ".entry") || match(TokenKind::Directive, ".func")) {
      m.functions.push_back(parseFunction());
    } else {
      ++idx_;
    }
  }
  return m;
}

} // namespace toygpu
