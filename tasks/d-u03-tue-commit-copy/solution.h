#pragma once
#include <algorithm>
#include <array>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

// Детерминированная ошибка копирования: size_t(-1) означает «не бросать».
inline int copy_item(int value, std::size_t index, std::size_t fail_after) {
  if (index == fail_after) throw std::bad_alloc();
  return value;
}
inline void replace_checked(std::vector<int>&, std::span<const int>, std::size_t);
