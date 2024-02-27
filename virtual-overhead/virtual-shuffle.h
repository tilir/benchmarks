//-----------------------------------------------------------------------------
//
// Virtualization benchmark, part 1
//
//-----------------------------------------------------------------------------

#pragma once

#include <array>
#include <algorithm>
#include <random>
#include <vector>

constexpr int ASZ = 10; // small array size to shuffle
constexpr int NCALL = 100;
constexpr int NBMK = 100;

struct NonVirt {
  std::array<int, ASZ> a;
  std::random_device rd;
  std::mt19937 g;
  NonVirt() : g(rd()) {}
  int foo(int x);
  int bar(int x);
};

struct VirtBase {
  std::array<int, ASZ> a;
  std::random_device rd;
  std::mt19937 g;
  VirtBase() : g(rd()) {}
  virtual int foo(int x);
  virtual int bar(int x);
};

struct VirtDerived : VirtBase {
  int foo(int x) override;
  int bar(int x) override;
};

void startup(NonVirt *V);
void startup(VirtBase *V);
