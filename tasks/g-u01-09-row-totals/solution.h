#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::int64_t> row_totals(const std::vector<int>& values, std::size_t rows, std::size_t cols);
