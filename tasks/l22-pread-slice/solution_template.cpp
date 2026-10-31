#include "solution.h"
#include <stdexcept>

std::string read_slice(int fd, off_t offset, std::size_t count, PreadFn rd) {
  (void)fd;
  (void)offset;
  (void)count;
  (void)rd;
  // TODO: реализуй контракт; main() предоставляет стенд.
  throw std::logic_error("TODO");
}
