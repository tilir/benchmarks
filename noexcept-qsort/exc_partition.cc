//-----------------------------------------------------------------------------
//
// Noexcept partition predicate benchmark
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

BENCHMARK_FIXTURE(ContainerFixture, "excpart") {
  auto pivot = container[0];
  auto excpred = [pivot](int x) { return x < pivot; };
  std::partition(container.begin(), container.end(), excpred);
}

BENCHMARK_FIXTURE(ContainerFixture, "nexcpart") {
  auto pivot = container[0];
  auto nexcpred = [pivot] (int x) noexcept { return x < pivot; };
  std::partition(container.begin(), container.end(), nexcpred);
}

BENCHMARK_MAIN()
