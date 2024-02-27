//-----------------------------------------------------------------------------
//
// Exeptions vs ret codes benchmark part 2
//
// see exc_ret_1.cc for part 1
//
//-----------------------------------------------------------------------------

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

#include "exc_ret.h"

int call_outer_exc(int x) {
  if (x <= 0)
    call_inner_exc(x);
  return call_exc(x - 1) + 1;
}

int call_outer_retc(int x) {
  int n;
  if (x <= 0)
    n = call_inner_retc(x);
  else
    n = call_retc(x - 1);
  if (n == -1)
    return -1;
  return n + 1;  
}

int startup_exc() {
  int sum = 0;
  int nerrs = 0;
  for (int i = 0; i < NBMK; ++i) {
    try {
      sum += call_exc(NCALL);
    } catch(std::runtime_error &e) {
      nerrs += 1;
    }
  }
  return sum;
}

int startup_retc() {
  int sum = 0;
  int nerrs = 0;
  for (int i = 0; i < NBMK; ++i) {
    int x = call_retc(NCALL);
    if (x == -1) {
      nerrs += 1;
    } else {
      sum += x;
    }
  }
  return sum;
}
