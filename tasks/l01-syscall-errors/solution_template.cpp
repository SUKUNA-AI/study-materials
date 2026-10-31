// l01-syscall-errors: сдаётся этот файл (solution.cpp). Linux (glibc), C++23.
#include "solution.h"

#include <cerrno>
#include <stdexcept>
#include <string>
#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace lab::sys {

long check(long ret, const char* what) {
  // TODO(L1): при ret == -1 — std::system_error(errno, std::generic_category(), what).
  // Почему errno нужно прочитать СРАЗУ, до любых других вызовов?
  (void)what;
  return ret;
}

void check_rc(int rc, const char* what) {
  // TODO(L1): у pthread-функций код ошибки — это rc, а не errno.
  (void)rc;
  (void)what;
}

std::int64_t file_size(const char* path) {
  // TODO(L1): open(O_RDONLY | O_CLOEXEC) через check, fstat, close — и не потерять дескриптор,
  // если что-то бросило между open и close.
  (void)path;
  throw std::logic_error("TODO: file_size");
}

}  // namespace lab::sys
