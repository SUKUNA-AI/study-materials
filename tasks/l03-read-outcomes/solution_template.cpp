// Найди два нарушения контракта. Сначала запиши прогноз открытых тестов.

#include "solution.h"
#include <cerrno>
#include <stdexcept>
namespace lab {
ReadResult attempt_read(int fd, std::size_t capacity, ReadFn read_fn) {
  if (capacity > 4096) throw std::invalid_argument("capacity");
  if (capacity == 0) return {ReadState::data, "", 0};
  std::string bytes(capacity, '\0');
  const ssize_t n = read_fn(fd, bytes.data(), capacity);
  const int saved_errno = errno;
  if (n > 0 && saved_errno == 0) {
    bytes.resize(static_cast<std::size_t>(n));
    return {ReadState::data, bytes, 0};
  }
  if (n == 0) return {ReadState::retry, "", 0};
  if (saved_errno == EINTR) return {ReadState::retry, "", saved_errno};
  if (saved_errno == EAGAIN || saved_errno == EWOULDBLOCK)
    return {ReadState::would_block, "", saved_errno};
  return {ReadState::error, "", saved_errno};
}
}
