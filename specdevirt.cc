#include <algorithm>
#include <random>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

constexpr int NCALL = 100;
constexpr int NBMK = 10000;

struct VirtBase {
  virtual int foo(int x) = 0;
  virtual int bar(int x) = 0;
};

struct VirtDerived : VirtBase {
  int foo(int x) override {
    return f(x);
  }

  int bar(int x) override {
    return foo(x);
  }
};

int __attribute__((noinline)) startup(NonVirt *nv) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += nv->bar(NCALL);
  return sum;
}

int __attribute__((noinline)) startup(VirtBase *vb) {
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
