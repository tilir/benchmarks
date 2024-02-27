//-----------------------------------------------------------------------------
//
// Ranges filter benchmark
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <iterator>
#include <exception>
#include <ranges>
#include <utility>
#include <vector>

namespace ranges = std::ranges;

constexpr int BMKS = 10000;

class RangesFixture {
protected:
  std::vector<int> container;
  std::vector<int> contout;

  RangesFixture() {
    container.reserve(BMKS);
    for (int i = 0; i < BMKS; ++i)
      container.push_back(rand());
  }
};

BENCHMARK_FIXTURE(RangesFixture, "naive") {
  contout.reserve(container.size());
  for (auto& elt : container)
    if (elt < 5)
      contout.push_back(elt * 2);
}

BENCHMARK_FIXTURE(RangesFixture, "planar") {
  std::vector<int> v;
  v.reserve(container.size());
  std::copy_if(container.begin(), container.end(), std::back_inserter(v),
               [](int n){ return n < 5; });
  contout.resize(v.size());
  std::transform(v.begin(), v.end(), contout.begin(),
                 [] (int n){ return n * 2; });
}

BENCHMARK_FIXTURE(RangesFixture,
                  "modern") {
  contout.reserve(container.size());
  auto v = ranges::views::all(container) | 
           ranges::views::filter([](int n){ return n < 5; }) | 
           ranges::views::transform([](int n){ return n * 2; });
  ranges::copy(v, contout.begin());
}

BENCHMARK_MAIN()