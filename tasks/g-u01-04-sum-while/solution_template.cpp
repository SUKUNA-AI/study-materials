#include "solution.h"

std::int64_t sum_to_n(int n) {
  std::int64_t total = 0;
  int next = 1;
  while (next < n) { total += next; ++next; }
  return total;
}
