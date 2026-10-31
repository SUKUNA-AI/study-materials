#pragma once
#include <cstdint>
#include <optional>
#include <vector>
struct Sample { int value; bool valid; };
std::optional<std::int64_t> valid_total(const std::vector<Sample>& samples);
