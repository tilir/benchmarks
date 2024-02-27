//-----------------------------------------------------------------------------
//
// Sum of fibonacci numbers benchmark, part 1
//
// clang++ --std=c++20 -O2 corofibs.cc sumfib_module.cc ${OPTIONS}
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include "corofibs.h"

generator<int> fibs() {
  int a = 1, b = 0;
  for (;;) {
    co_yield b;
    b = std::exchange(a, (a + b) % FM) % FM;
  }
}

int sumfib_coro(int n) {
  int sum = 0;
  auto nums = fibs();

  for (int i = 0; i < n; ++i) {
    nums.move_next();
    auto y = nums.current_value();    
    sum = sum + y;
  }

  return sum;
}

int sumfib(int n) {
  int sum = 0;
  int a = 1, b = 0;

  for (int i = 0; i < n; ++i) {    
    sum = sum + b;
    b = std::exchange(a, (a + b) % FM) % FM;
  }

  return sum;
}

BENCHMARK("sumfib_coro") {
  startup_coro(NBMK);
}

BENCHMARK("sumfib") {
  startup(NBMK);
}

BENCHMARK_MAIN()
