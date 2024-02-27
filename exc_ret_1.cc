//-----------------------------------------------------------------------------
//
// Exeptions vs ret codes benchmark part 1
//
// see exc_ret_2.cc for part 2
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

#ifndef NOBMK
#include "benchmark/cppbenchmark.h"
#endif

#include "exc_ret.h"

int call_exc(int x) {
  return call_outer_exc(x - 1) + 1;
}

int call_inner_exc(int x) {
  throw std::runtime_error("something");
}

int call_retc(int x) {
  int n = call_outer_retc(x - 1);
  if (n == -1)
    return -1;
  return n + 1;
}

int call_inner_retc(int x) {
  return -1;
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
