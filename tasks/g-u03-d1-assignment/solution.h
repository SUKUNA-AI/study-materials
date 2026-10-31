#pragma once
#include <string>
#include <string_view>
#include <optional>
#include <utility>

std::optional<std::pair<std::string,std::string>> parse_assignment(std::string_view text);
