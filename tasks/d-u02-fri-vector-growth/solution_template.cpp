#include "solution.h"

inline void append_marker(std::vector<int>& v, int delta, int marker) {
  int* first = v.empty() ? nullptr : &v.front();
  v.reserve(v.capacity()+1);
  v.push_back(marker);
  if (first) *first += delta;
}
