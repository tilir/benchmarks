//-----------------------------------------------------------------------------
//
// Virtualization benchmark, part 1
//
//-----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <random>
#include <vector>

constexpr int NCALL = 100;
constexpr int NBMK = 10000;

struct NonVirt {
  int foo(int x);
  int bar(int x);
};

struct VirtBase {
  virtual int foo(int x);
  virtual int bar(int x);
};

struct VirtDerived : VirtBase {
  int foo(int x) override;
  int bar(int x) override;
};

void startup(NonVirt *V);
void startup(VirtBase *V);
