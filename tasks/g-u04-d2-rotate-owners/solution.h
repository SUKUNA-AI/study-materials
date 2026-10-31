#pragma once
#include <array>
#include <memory>

void rotate_owners(std::array<std::unique_ptr<int>,3>& owners);
