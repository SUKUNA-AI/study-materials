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

struct Job {
  int value;
  inline static int live = 0;
  explicit Job(int v) : value(v) { ++live; }
  Job(const Job&) = delete;
  Job& operator=(const Job&) = delete;
  ~Job() { --live; }
};
inline std::unique_ptr<Job> replace_job(std::unique_ptr<Job>&, std::unique_ptr<Job>&);
