#pragma once
#include <optional>
#include <vector>
struct Sample { int id; int value; bool valid; };
std::optional<int> best_sample(const std::vector<Sample>& samples);
