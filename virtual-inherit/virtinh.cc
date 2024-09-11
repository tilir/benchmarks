//-----------------------------------------------------------------------------
//
// Virtual inheritance benchmark
//
// needs fmodule.cc
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <random>
#include <sys/cdefs.h>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

constexpr int NCALL = 100;
constexpr int NBMK = 10000;

int f(int);

struct VirtBase { virtual int foo(int x) { return f(x); } };
struct B : virtual public VirtBase {};
struct C : virtual public VirtBase {};
struct VirtDerived : public B, public C {
  int foo(int x) override { return f(x) + 1; };
};

struct Base { virtual int foo(int x) { return f(x); } };
struct Derived : Base {
  int foo(int x) override { return f(x) + 1; }
};

int __attribute__((noinline)) startup(Base *b) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += b->foo(NCALL);
  return sum;
}

int __attribute__((noinline)) startup(VirtBase *vb) {
  int sum = 0;
  for (int i = 0; i < NBMK; ++i)
    sum += vb->foo(NCALL);
  return sum;
}

#ifndef NOBMK

BENCHMARK("virtualinh") {
  Derived nv;
  startup(&nv);
}

BENCHMARK("nvirtualinh") {
  VirtDerived vd;
  startup(&vd);
}

BENCHMARK_MAIN()

#endif
