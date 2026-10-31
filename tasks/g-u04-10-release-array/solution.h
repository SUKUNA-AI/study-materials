#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
int* hand_out(std::unique_ptr<int[]>& owner);
