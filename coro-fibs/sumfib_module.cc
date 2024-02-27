//-----------------------------------------------------------------------------
//
// Sum of fibonacci numbers benchmark, part 2
//
//-----------------------------------------------------------------------------

#include "corofibs.h"

int startup_coro(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i)
    sum += sumfib_coro(NFIB);
  return sum;
}

int startup(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i)
    sum += sumfib(NFIB);
  return sum;
}