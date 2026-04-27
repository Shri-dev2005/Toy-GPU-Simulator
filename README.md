# Toy GPU Simulator (C++17)

A modular, educational GPU compiler + runtime simulator inspired by PTX and LLVM-style compiler design.

This project parses PTX-like assembly, builds a structured IR, runs optimization passes, executes instructions in a simplified SIMT model, simulates memory hierarchy behavior, and reports performance metrics.

---

## 1) Goals

The simulator is designed to be:

- **Compiler-oriented**: includes frontend, IR, validation, and optimization stages.
- **Architecture-oriented**: includes warp-level execution and memory hierarchy analysis.
- **Modular**: each subsystem is isolated and replaceable.
- **Dependency-light**: pure C++17 + STL (no mandatory external libraries).

---

## 2) High-Level Pipeline

```text
PTX-like source
   │
   ▼
Lexer ──► Parser ──► IR Module/Function/BasicBlock/Instruction
   │                     │
   │                     ▼
   │               Semantic Validation
   │                     │
   ▼                     ▼
Diagnostics         PassManager (CF, DCE, RA, Scheduling)
                           │
                           ▼
                    Linearized instruction stream
                           │
                           ▼
               SIMT Execution Engine (WarpScheduler)
                           │
                           ▼
            Performance summary + memory behavior analysis
```

---

## 3) Repository Layout

```text
.
├── include/toygpu/          # Public subsystem interfaces
├── ptx-frontend/            # Types, IR helpers, lexer, parser, semantic checks
├── optimizer/               # Pass infra + optimization passes
├── exec-engine/             # Warp state, ALU, branch handling, scheduler
├── memory/                  # Address spaces, cache model, access analyzers
├── debug/                   # Logger, diagnostics renderer, graphviz CFG dumper
├── examples/                # PTX-like sample inputs
├── tests/                   # Lightweight STL-based test harness + subsystem tests
├── CMakeLists.txt           # Build configuration
└── main.cpp                 # End-to-end driver
```

---

## 4) Subsystem Architecture (Detailed)

### 4.1 PTX Frontend

#### Type System
- `PTXType` enum provides primitive scalar + predicate kinds.
- `TypeInfo` captures bit width and semantic properties (signed/float/predicate).
- Utility functions parse type tokens (e.g. `.s32`, `.f32`) and check compatibility.

#### IR Model
- `Value`: virtual register or literal.
- `Instruction`: opcode + operands + optional predicate + branch target label.
- `BasicBlock`: labeled instruction sequence with predecessor/successor lists.
- `Function`: collection of blocks and register declarations.
- `Module`: collection of functions.

This yields an LLVM-inspired hierarchical IR with explicit CFG edges.

#### Lexer
Converts source text to token stream with support for:
- directives (e.g. `.entry`, `.reg`, `.s32`)
- registers (e.g. `%r1`, `%p0`)
- literals, delimiters, labels
- comments (`//`)

#### Recursive-Descent Parser
Builds IR in two conceptual stages:
1. Parse function/register/block/instruction structures.
2. Resolve control flow by linking branch/fallthrough edges into CFG predecessors/successors.

It supports predicated syntax like `@%p0 bra done;`.

#### Semantic Validator
Checks:
- undefined register usage
- predicate register validity
- branch target existence

Diagnostics are gathered for structured error reporting.

---

### 4.2 Optimizer

#### Pass Infrastructure
- `FunctionPass` / `ModulePass` interfaces
- `PassManager` executes configured pipeline on the module/functions

#### Included Passes
1. **Constant Folding**
   - folds constant arithmetic (`add/mul/sub`) to immediate `mov`
2. **Dead Code Elimination (DCE)**
   - reverse scan + simple liveness set
   - retains side-effecting instructions
3. **Register Allocation (graph-based skeleton)**
   - builds interference-like graph from liveness
   - applies greedy coloring approximation
   - provides DOT graph hook for visualization
4. **Instruction Scheduler**
   - simple list-style reordering policy with side-effect awareness

> Note: this repository intentionally keeps algorithms understandable and compact; it is a pedagogical baseline for deeper compiler experimentation.

---

### 4.3 Execution Engine (SIMT)

#### WarpState
Models one warp of **32 lanes**:
- lane-local register file `[32][128]`
- per-lane active mask
- divergence/reconvergence stack
- program counter + progress counters

#### ALU
Executes ALU ops per active lane:
- `mov`, `add`, `mul`, `sub` (current implementation)

#### Branch Unit
Tracks branch/divergence events and manipulates active masks and reconvergence metadata.

#### Warp Scheduler
Cycle-based dispatcher:
- issues instructions in program order
- injects stall cycles for memory operations
- invokes ALU / branch unit
- emits `PerfStats` (cycles, issued, stalls, IPC)

---

### 4.4 Memory Subsystem

#### Address Spaces
`AddressSpace` emulates separate regions:
- GLOBAL
- SHARED
- CONSTANT
- LOCAL

Includes typed 32-bit loads/stores with bounds checks.

#### Cache Model
Set-associative cache with LRU replacement policy:
- configurable size, line size, ways
- hit/miss accounting

#### Coalescing Analyzer
Determines whether warp addresses collapse into one/few transactions based on line mapping.

#### Bank Conflict Analyzer
Models 32 shared-memory banks and reports:
- worst conflict degree
- serialized accesses

---

### 4.5 Debug + Diagnostics

- `Logger`: level-filtered logging helper.
- `DiagnosticsEngine`: aggregates frontend semantic diagnostics.
- `dumpCFGDot`: emits Graphviz DOT for function CFG visualization.

---

## 5) Build and Run

### Prerequisites
- CMake >= 3.16
- C++17 compiler (GCC/Clang/MSVC with C++17 support)

### Build

```bash
cmake -S . -B build
cmake --build build -j
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

### Run simulator (default sample)

```bash
./build/toygpu_sim
```

### Run simulator with custom input

```bash
./build/toygpu_sim examples/vector_add.ptx
```

---

## 6) Example Program

`examples/vector_add.ptx` shows:
- register declarations
- predicated branch
- arithmetic sequence
- labeled control flow

Use it to validate parser/validator/pass/execution integration end-to-end.

---

## 7) Runtime Output Explained

The driver prints:
- **Cycles**: simulated total cycles
- **Issued Instructions**: count of instructions dispatched
- **IPC**: issued/cycles ratio
- **Stall Cycles**: scheduler-inserted stalls (e.g., memory)
- **Divergent Branches**: branch divergence events
- **Coalesced/Transactions**: memory access efficiency proxy
- **Shared Bank Conflict Stats**: conflict serialization pressure
- **CFG DOT**: graph description for visualization tools

---

## 8) Extending the Simulator

Recommended next upgrades:

1. **Frontend**
   - richer PTX grammar (addressing modes, call args, directives)
   - stronger type propagation and instruction typing

2. **IR + Analysis**
   - explicit def-use chains
   - dominance / SSA utilities

3. **Optimizer**
   - canonical CFG simplification
   - GVN/CSE, LICM, strength reduction
   - true Chaitin-Briggs allocator with spill code insertion

4. **Execution**
   - CTA/block scheduling model
   - realistic reconvergence rules
   - latencies per opcode class

5. **Memory**
   - unified L1/L2 interaction model
   - write policies and miss penalties
   - shared-memory timing model

6. **Tooling**
   - richer trace output and per-warp timeline
   - JSON/perf export for dashboarding

---

## 9) Testing Strategy

The `tests/` folder uses a small dependency-free harness and includes checks for:
- frontend parsing
- constant folding
- warp execution behavior
- address-space/cache behavior

This keeps CI setup lightweight while allowing incremental subsystem verification.

---

## 10) License / Intended Use

This project is educational and intended for architecture/compiler learning, prototyping, and experimentation.

