#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
std::unique_ptr<int[]> resize_copy(const std::unique_ptr<int[]>& source, std::size_t old_size, std::size_t new_size);
