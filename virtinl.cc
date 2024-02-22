#include <algorithm>
#include <random>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

constexpr int NCALL = 100;
constexpr int NBMK = 10000;

int g;
int f(int);

struct NonVirt {
  int foo(int x) {
    g += f(x);
    return (x > 0) ? bar(x - 1) : 0;
  }

  int bar(int x) {
    g += f(x);
    return (x > 0) ? foo(x - 1) : 0;
  }
};

struct VirtBase {
  virtual int foo(int x) {
    g += f(x);
    return (x > 0) ? bar(x - 1) : 0;
  }

  virtual int bar(int x) {
    g += f(x);
    return (x > 0) ? foo(x - 1) : 0;
  }
};

struct VirtDerived : VirtBase {
  int foo(int x) override {
    g += f(x);
    return (x > 0) ? bar(x - 1) : 0;
  }

  int bar(int x) override {
    g += f(x);
    return (x > 0) ? foo(x - 1) : 0;
  }
};

void __attribute__((noinline)) startup(NonVirt *nv) {
  for (int i = 0; i < NBMK; ++i) {
    g = f(g);
    nv->bar(NCALL);
  }
}

void __attribute__((noinline)) startup(VirtBase *vb) {
  for (int i = 0; i < NBMK; ++i) {
    g = f(g);
    vb->bar(NCALL);
  }
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
