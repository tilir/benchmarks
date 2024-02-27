//-----------------------------------------------------------------------------
//
// Exeptions vs ret codes benchmark, module 1
//
// exception vs error codes:
// clang++ -O2 exc_ret_1.cc exc_ret_2.cc ${OPTIONS}
//
// no exception vs no error codes
// clang++ -O2 -DNOEXC exc_ret_1.cc exc_ret_2.cc ${OPTIONS}
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
#ifndef NOEXC
  throw std::runtime_error("something");
#else
  return 0;
#endif
}

int call_retc(int x) noexcept {
  int n = call_outer_retc(x - 1);
  if (n == -1)
    return -1;
  return n + 1;
}

int call_inner_retc(int x) noexcept {
#ifndef NOEXC
  return -1;
#else
  return 0;
#endif
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
