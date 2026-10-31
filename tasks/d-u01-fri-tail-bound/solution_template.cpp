#include "solution.h"

inline std::int64_t tail_sum(std::span<const int> v, std::size_t start) {
  std::int64_t total = 0;
  for (std::size_t i=start; i<=v.size(); ++i) total += v[i];
  return total;
}
