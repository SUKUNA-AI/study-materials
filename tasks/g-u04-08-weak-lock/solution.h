#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
std::optional<int> weak_value(const std::weak_ptr<int>& observer);
