#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
std::array<std::shared_ptr<int>, 2> share_twice(const std::shared_ptr<int>& owner);
