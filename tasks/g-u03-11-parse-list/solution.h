#pragma once
#include <optional>
#include <vector>
#include <string_view>

std::optional<std::vector<int>> parse_integer_list(std::string_view text);
