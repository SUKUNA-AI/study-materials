#pragma once
#include <string>
#include <vector>
#include <utility>
#include <format>
#include <cstddef>

std::string render_table(const std::vector<std::pair<std::string,int>>& rows);
