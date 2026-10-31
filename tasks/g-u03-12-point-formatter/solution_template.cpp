#include "solution.h"
#include <format>

template<> struct std::formatter<Point> {
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
  auto format(const Point& p, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "({}, {})", p.x, 0);
  }
};
std::string format_point(int x, int y) { return std::format("{}", Point{x,y}); }
