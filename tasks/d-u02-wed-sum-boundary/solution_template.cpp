#include "solution.h"

inline int sum_to_n(int n) {
  int sum = 0;
  // Здесь одна ошибка в границе цикла. Найди её трассировкой.
  for (int i = 1; i < n; ++i) sum += i;
  return sum;
}
