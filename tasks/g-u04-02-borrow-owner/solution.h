#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
int read_or(const std::unique_ptr<int>& owner, int fallback);
