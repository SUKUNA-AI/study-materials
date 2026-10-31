#pragma once
#include <optional>
#include <string_view>
#include <charconv>
#include <system_error>

std::optional<unsigned> parse_rgb(std::string_view text);
