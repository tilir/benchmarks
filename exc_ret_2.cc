#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

#include "exc_ret.h"

int call_outer_exc(int x) {
  int sum = 0;
  for (int i = 0; i < NCALL; ++i)
    sum += call_inner_exc(x - i / 2);
  return sum;
}

int call_outer_retc(int x) {
  int sum = 0;
  for (int i = 0; i < NCALL; ++i) {
    int n = call_inner_retc(x - i);
    if (n == -1)
      return -1;
    if (n == -2)
      return -2;
    if (n == -3)
      return -3;
    sum += n;
  }
  return sum;
}