#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

#include "exc_ret.h"

int call_exc(int x) {
  int sum = 0;
  for (int i = 0; i < NCALL; ++i)
    sum += call_outer_exc(x - i / 2);
  return sum;
}

int call_inner_exc(int x) {
  if (x < 0)
    throw std::runtime_error("something");
  return 1;
}

int call_retc(int x) {
  int sum = 0;
  for (int i = 0; i < NCALL; ++i) {
    int n = call_outer_retc(x - i);
    if (n == -1)
      return -1;
    if (n == -2)
      return -2;
    if (n == -3)
      return -3;
    sum += n;
  }
  return sum;
}

int call_inner_retc(int x) {
  if (x < 0)
    return -1;
  return 1;
}

int __attribute__((noinline)) startup_exc() {
  int sum = 0;
  int nerrs = 0;
  for (int i = 0; i < NBMK; ++i) {
    try {
      sum += call_exc(NCALL);
    } catch(std::runtime_error &e) {
      nerrs += 1;
    }
  }
  return sum;
}

int __attribute__((noinline)) startup_retc() {
  int sum = 0;
  int nerrs = 0;
  for (int i = 0; i < NBMK; ++i) {
    int x = call_retc(NCALL);
    if (x == -1) {
      nerrs += 1;
    } else {
      sum += x;
    }
  }
  return sum;
}

#ifndef NOBMK

BENCHMARK("exception") {
  startup_exc();
}

BENCHMARK("retcodes") {
  startup_retc();
}

BENCHMARK_MAIN()

#endif
