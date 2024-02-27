//-----------------------------------------------------------------------------
//
// Sum of fibonacci numbers benchmark, part 2
//
// for part 1 see corofibs.cc
//
//-----------------------------------------------------------------------------

constexpr int NFIB = 100;

int sumfib_coro(int n);
int sumfib(int n);

int startup_coro(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i)
    sum += sumfib_coro(10);
  return sum;
}

int startup(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i)
    sum += sumfib(10);
  return sum;
}