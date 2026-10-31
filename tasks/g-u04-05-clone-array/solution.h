#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
std::unique_ptr<int[]> clone_values(const int* data, std::size_t count);
