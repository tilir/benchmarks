//-----------------------------------------------------------------------------
//
// Virtualization benchmark, part 1
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <random>
#include <vector>

#include "virtual.h"

int NonVirt::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
}

int VirtBase::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
};

int VirtDerived::foo(int x) {
  return (x > 0) ? bar(x - 1) : 0;
}

void startup(NonVirt *nv) {
  for (int i = 0; i < NBMK; ++i)
    nv->bar(NCALL);
}

void startup(VirtBase *vb) {
  for (int i = 0; i < NBMK; ++i)
    vb->bar(NCALL);
}
