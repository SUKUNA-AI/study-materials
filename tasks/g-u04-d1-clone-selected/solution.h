#pragma once
#include <vector>
#include <memory>
#include <cstddef>

std::vector<std::unique_ptr<int>> clone_positive(const std::vector<std::unique_ptr<int>>& values);
