#include <algorithm>
#include <random>
#include <vector>

#include "benchmark/cppbenchmark.h"

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

void startup(NonVirt *nv) {
  for (int i = 0; i < NBMK; ++i)
    nv->bar(NCALL);
}

void startup(VirtBase *vb) {
  for (int i = 0; i < NBMK; ++i)
    vb->bar(NCALL);
}

int NonVirt::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
}

int VirtBase::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
};

int VirtDerived::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
}

int NonVirt::bar(int x) {
  return (x > 0) ? foo(x - 1) : 0;
}

int VirtBase::bar(int x) {
  return (x > 0) ? foo(x - 1) : 0;
};

int VirtDerived::bar(int x) {
  return (x > 0) ? foo(x - 1) : 0;
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