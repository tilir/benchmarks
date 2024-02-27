//-----------------------------------------------------------------------------
//
// Virtualization benchmark, part 1
//
//-----------------------------------------------------------------------------

#ifndef NOBMK

#include "benchmark/cppbenchmark.h"

#endif

#include <algorithm>
#include <random>
#include <vector>

#include "virtual-shuffle.h"

int NonVirt::bar(int x) {
  std::shuffle(a.begin(), a.end(), g);
  return (x > 0) ? foo(x - 1) : 0;
}

int VirtBase::bar(int x) {
  std::shuffle(a.begin(), a.end(), g);
  return (x > 0) ? foo(x - 1) : 0;
};

int VirtDerived::bar(int x) {
  std::shuffle(a.begin(), a.end(), g);
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