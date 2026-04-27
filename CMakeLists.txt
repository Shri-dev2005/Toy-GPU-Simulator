cmake_minimum_required(VERSION 3.16)
project(ToyGPUSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

set(FRONTEND_SOURCES
  ptx-frontend/TypeSystem.cpp
  ptx-frontend/IR.cpp
  ptx-frontend/Lexer.cpp
  ptx-frontend/Parser.cpp
  ptx-frontend/SemanticValidator.cpp
)

set(OPT_SOURCES
  optimizer/Pass.cpp
  optimizer/DeadCodeElimination.cpp
  optimizer/ConstantFolding.cpp
  optimizer/RegisterAllocation.cpp
  optimizer/InstructionScheduler.cpp
)

set(EXEC_SOURCES
  exec-engine/WarpState.cpp
  exec-engine/ALU.cpp
  exec-engine/BranchUnit.cpp
  exec-engine/WarpScheduler.cpp
)

set(MEM_SOURCES
  memory/AddressSpace.cpp
  memory/CacheModel.cpp
  memory/CoalescingChecker.cpp
  memory/BankConflictDetector.cpp
)

set(DEBUG_SOURCES
  debug/Logger.cpp
  debug/Diagnostics.cpp
  debug/GraphvizDumper.cpp
)

add_library(toygpu
  ${FRONTEND_SOURCES}
  ${OPT_SOURCES}
  ${EXEC_SOURCES}
  ${MEM_SOURCES}
  ${DEBUG_SOURCES}
)

target_include_directories(toygpu PUBLIC include)

add_executable(toygpu_sim main.cpp)
target_link_libraries(toygpu_sim PRIVATE toygpu)

enable_testing()
add_executable(toygpu_tests
  tests/TestMain.cpp
  tests/FrontendTests.cpp
  tests/OptimizerTests.cpp
  tests/ExecutionTests.cpp
  tests/MemoryTests.cpp
)
target_link_libraries(toygpu_tests PRIVATE toygpu)
add_test(NAME toygpu_tests COMMAND toygpu_tests)
