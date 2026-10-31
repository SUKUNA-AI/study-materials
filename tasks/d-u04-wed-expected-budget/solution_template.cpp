#include "solution.h"

inline std::expected<std::uint32_t,ParseError> parse_budget(std::string_view) { return std::unexpected(ParseError::empty); }
