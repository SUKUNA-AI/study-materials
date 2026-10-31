#include "solution.h"

template<class T> T& checked_at(std::span<T>, std::ptrdiff_t) { throw std::out_of_range("TODO"); }
