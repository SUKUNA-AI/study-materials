#pragma once
#include <optional>
enum class Mode { disabled, fixed, automatic };
std::optional<int> selected_limit(Mode mode, std::optional<int> supplied);
