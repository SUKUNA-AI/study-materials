#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
std::unique_ptr<int[]> make_ramp(std::size_t count, int start);
