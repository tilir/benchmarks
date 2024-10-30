//-----------------------------------------------------------------------------
//
// Virtual inline benchmark
//
// needs fmodule.cc
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <random>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

#if defined(_WIN32) || defined(_WIN64)
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#endif

constexpr int NCALL = 100;
constexpr int NBMK = 10000;

int f(int);

struct NonVirt {
  int foo(int x) {
    return f(x);
  }

  int bar(int x) {
    return foo(x);
  }
};

struct VirtBase {
  virtual int foo(int x) {
    return f(x);
  }

  virtual int bar(int x) {
    return foo(x);
  }
};

struct VirtDerived : VirtBase {
  int foo(int x) override {
    return f(x);
  }

  int bar(int x) override {
    return foo(x);
  }
};

NOINLINE int startup(NonVirt *nv) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += nv->bar(NCALL);
  return sum;
}

NOINLINE int startup(VirtBase *vb) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += vb->bar(NCALL);
  return sum;
}

#ifndef NOBMK

BENCHMARK("nvirtualcall") {
  NonVirt nv;
  startup(&nv);
}

BENCHMARK("virtualcall") {
  VirtDerived vd;
  startup(&vd);
}

BENCHMARK_MAIN()

#endif
