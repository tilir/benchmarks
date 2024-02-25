#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <list>
#include <vector>

constexpr int NELTS = 1000000;

bool excpred(int x, int y) {
  return x < y;
}

bool nexcpred(int x, int y) noexcept {
  return x < y;
}

class ContainerFixture {
protected:
  std::vector<int> container;

  ContainerFixture() {
    for (int i = 0; i < NELTS; ++i)
      container.push_back(rand());
  }
};

BENCHMARK_FIXTURE(ContainerFixture, "excqsort") {
  std::sort(container.begin(), container.end(), excpred);
}

BENCHMARK_FIXTURE(ContainerFixture, "nexcqsort") {
  std::sort(container.begin(), container.end(), nexcpred);
}

BENCHMARK_MAIN()
