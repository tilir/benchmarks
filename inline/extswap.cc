//-----------------------------------------------------------------------------
//
// Partition benchmark with external swap, module 2
//
//-----------------------------------------------------------------------------

#include "extswap.h"

void ext_swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}