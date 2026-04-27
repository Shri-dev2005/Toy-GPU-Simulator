#include "TestHarness.h"
#include <exception>
#include <iostream>

std::vector<std::pair<std::string, TestFn>>& testRegistry() {
  static std::vector<std::pair<std::string, TestFn>> tests;
  return tests;
}

TestRegistrar::TestRegistrar(std::string name, TestFn fn) { testRegistry().push_back({std::move(name), std::move(fn)}); }

int main() {
  int failed = 0;
  for (auto& [name, fn] : testRegistry()) {
    try { fn(); std::cout << "[PASS] " << name << "\n"; }
    catch (const std::exception& ex) { ++failed; std::cout << "[FAIL] " << name << ": " << ex.what() << "\n"; }
  }
  return failed;
}
