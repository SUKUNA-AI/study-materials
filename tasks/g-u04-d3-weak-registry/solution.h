#pragma once
#include <vector>
#include <memory>
#include <cstdint>

std::int64_t sum_live(const std::vector<std::weak_ptr<int>>& refs);
