#include <algorithm>
#include <random>
#include <ranges>
#include <vector>

#include "benchmark/cppbenchmark.h"

namespace ranges = std::ranges;
namespace views = std::views;

struct S { int x, y; };

constexpr int N = 100;
constexpr int NBMK = 1000;

class TestFixture {
protected:
  std::vector<S> v;
  std::vector<int> w;
  std::random_device r;
  std::mt19937 g;
  
  TestFixture() : g(r()) {
    v.resize(N);
    w.resize(N);
    std::uniform_int_distribution<int> dist {0, 100};
    auto gen = [&, this]() -> S { return {dist(g), dist(g)}; };
    std::generate(std::begin(v), std::end(v), gen);
  }
};

BENCHMARK_FIXTURE(TestFixture, "TransformComparator") {
  for (int i = 0; i < NBMK; ++i)
    ranges::transform(v.begin(), v.end(), w.begin(), [](const auto& a) { return a.x * 2; });
}

BENCHMARK_FIXTURE(TestFixture, "TransformProjector") {
  for (int i = 0; i < NBMK; ++i)
    ranges::transform(v.begin(), v.end(), w.begin(), [](auto x) { return x * 2; }, &S::x);
}

BENCHMARK_MAIN()
