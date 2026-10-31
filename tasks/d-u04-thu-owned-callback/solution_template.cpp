#include "solution.h"

inline std::move_only_function<int(int)> make_scale(std::unique_ptr<int>) { return {}; }
