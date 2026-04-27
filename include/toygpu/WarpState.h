#pragma once
#include <array>
#include <cstdint>
#include <stack>
#include <vector>

namespace toygpu {

struct ReconvergencePoint {
  std::size_t pc;
  std::array<bool, 32> mask;
};

struct WarpState {
  std::size_t pc{0};
  std::array<std::array<int32_t, 128>, 32> registers{};
  std::array<bool, 32> activeMask{};
  std::vector<ReconvergencePoint> divergenceStack;
  std::size_t completedInstructions{0};

  WarpState();
};

} // namespace toygpu
