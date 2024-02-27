//-----------------------------------------------------------------------------
//
// Partition benchmark, part 1
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include <algorithm>
#include <random>
#include <vector>

constexpr int MAX = 10000;
constexpr int NPTS = 10000;

std::random_device rd;
std::mt19937 gen(rd());

void ext_swap(int *a, int *b);

unsigned partition_extswap(int *arr, unsigned low, unsigned high) {
  int pivot, i, j;
  pivot = arr[low];
  i = high;

  for (j = high; j > low; j--)
    if (arr[j] > pivot) {
      ext_swap(arr + i, arr + j);
      i -= 1;
    }

  ext_swap(arr + i, arr + low);
  return i;
}

unsigned partition_iterswap(int *arr, unsigned low, unsigned high) {
  int pivot, i, j;
  pivot = arr[low];
  i = high;

  for (j = high; j > low; j--)
    if (arr[j] > pivot) {
      std::iter_swap(arr + i, arr + j);
      i -= 1;
    }

  std::iter_swap(arr + i, arr + low);
  return i;
}

class TestFixture {
protected:
  std::vector<int> v;

  TestFixture() {
    std::uniform_int_distribution<> distrib(0, MAX);
    for (int i = 0; i < NPTS; ++i)
      v.push_back(distrib(gen));
  }
};

BENCHMARK_FIXTURE(TestFixture, "extswap")
{
  partition_extswap(&v[0], 0, v.size() - 1);
}

BENCHMARK_FIXTURE(TestFixture, "intswap")
{
  partition_iterswap(&v[0], 0, v.size() - 1);
}

BENCHMARK_MAIN()