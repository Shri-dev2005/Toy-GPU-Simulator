#include <fstream>
#include <iostream>
#include <sstream>

#include "toygpu/Diagnostics.h"
#include "toygpu/GraphvizDumper.h"
#include "toygpu/Lexer.h"
#include "toygpu/Memory.h"
#include "toygpu/Parser.h"
#include "toygpu/Pass.h"
#include "toygpu/SemanticValidator.h"
#include "toygpu/WarpScheduler.h"
#include "toygpu/WarpState.h"

using namespace toygpu;

int main(int argc, char** argv) {
  std::string inputPath = argc > 1 ? argv[1] : "examples/vector_add.ptx";
  std::ifstream in(inputPath);
  if (!in) {
    std::cerr << "Unable to open " << inputPath << "\n";
    return 1;
  }
  std::stringstream ss;
  ss << in.rdbuf();

  Lexer lexer(ss.str());
  auto tokens = lexer.tokenize();
  Parser parser(tokens);
  Module module = parser.parseModule();

  SemanticValidator validator;
  DiagnosticsEngine diags;
  for (const auto& d : validator.validate(module)) diags.add(d);
  if (diags.hasErrors()) {
    std::cerr << diags.render();
    return 1;
  }

  PassManager pm;
  pm.addFunctionPass(std::make_unique<ConstantFoldingPass>());
  pm.addFunctionPass(std::make_unique<DeadCodeEliminationPass>());
  pm.addFunctionPass(std::make_unique<InstructionSchedulerPass>());
  pm.addFunctionPass(std::make_unique<RegisterAllocationPass>());
  pm.run(module);

  if (module.functions.empty() || module.functions[0].blocks.empty()) return 0;
  std::vector<Instruction> linearProgram;
  for (const auto& bb : module.functions[0].blocks) {
    for (const auto& inst : bb.instructions) linearProgram.push_back(inst);
  }

  WarpState warp;
  WarpScheduler scheduler;
  auto perf = scheduler.run(linearProgram, warp);

  std::vector<std::size_t> accesses;
  for (std::size_t lane = 0; lane < 32; ++lane) accesses.push_back(lane * 4);
  auto coalescing = analyzeCoalescing(accesses);
  auto banks = analyzeBankConflicts(accesses);

  std::cout << "=== Toy GPU Performance Summary ===\n";
  std::cout << "Cycles: " << perf.cycles << "\n";
  std::cout << "Issued Instructions: " << perf.issuedInstructions << "\n";
  std::cout << "IPC: " << perf.ipc() << "\n";
  std::cout << "Stall Cycles: " << perf.stallCycles << "\n";
  std::cout << "Divergent Branches: " << scheduler.divergenceStats().divergentBranches << "\n";
  std::cout << "Coalesced: " << (coalescing.coalesced ? "yes" : "no") << ", Transactions: " << coalescing.transactions << "\n";
  std::cout << "Shared Bank Max Conflict: " << banks.maxConflict << ", Serialized Accesses: " << banks.serializedAccesses << "\n";
  std::cout << "\nCFG DOT:\n" << dumpCFGDot(module.functions[0]);

  return 0;
}
