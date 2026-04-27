#pragma once
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using TestFn = std::function<void()>;
std::vector<std::pair<std::string, TestFn>>& testRegistry();

struct TestRegistrar {
  TestRegistrar(std::string name, TestFn fn);
};

#define TEST(name) \
  void name(); \
  static TestRegistrar registrar_##name(#name, name); \
  void name()

#define ASSERT_TRUE(x) do { if (!(x)) throw std::runtime_error("assert true failed: " #x); } while (0)
#define ASSERT_EQ(a,b) do { if (!((a) == (b))) throw std::runtime_error("assert eq failed"); } while (0)
