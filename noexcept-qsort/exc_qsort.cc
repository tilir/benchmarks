//-----------------------------------------------------------------------------
//
// Noexcept qsort predicate benchmark
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <list>
#include <vector>

constexpr int NELTS = 10000;

class ContainerFixture {
protected:
  std::vector<int> container;

  ContainerFixture() {
    container.reserve(NELTS);
    for (int i = 0; i < NELTS; ++i)
      container.push_back(rand());
  }
};

BENCHMARK_FIXTURE(ContainerFixture, "excqsort") {
  auto excpred = [](int x, int y) { return x < y; };
  std::sort(container.begin(), container.end(), excpred);
}

BENCHMARK_FIXTURE(ContainerFixture, "nexcqsort") {
  auto nexcpred = [] (int x, int y) noexcept { return x < y; };
  std::sort(container.begin(), container.end(), nexcpred);
}

BENCHMARK_MAIN()
