#include "solution.h"

// Собери тело из строк ниже. Выбери две нужные строки, остальные — ловушки.
//
//     return value;
//     if (value > 0) return 0;
//     if (value < 0) return 0;
//     return -value;
//
inline int nonnegative(int value) {
  (void)value;
  return -1;
}
