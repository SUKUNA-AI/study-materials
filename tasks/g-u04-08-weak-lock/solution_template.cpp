#include "solution.h"
#include <stdexcept>

std::optional<int> weak_value(const std::weak_ptr<int>& observer) {
  (void)observer;
  // TODO: реализуй контракт; main() предоставляет стенд.
  throw std::logic_error("TODO");
}
