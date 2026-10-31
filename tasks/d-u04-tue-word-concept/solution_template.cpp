#include "solution.h"

template<class T> concept Word = std::integral<T>;
template<Word T> std::size_t storage_bytes(std::span<const T> values) { return values.size(); }
