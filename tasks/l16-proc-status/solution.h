#pragma once
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Готовая инфраструктура. Реализуй только объявления ниже.
namespace lab::support {
[[noreturn]] inline void fail(const char* operation) {
  const int saved = errno;
  throw std::system_error(saved, std::generic_category(), operation);
}
inline int checked(int rc, const char* operation) {
  if (rc == -1) fail(operation);
  return rc;
}
inline void checked_pthread(int rc, const char* operation) {
  if (rc != 0) throw std::system_error(rc, std::generic_category(), operation);
}
class Fd {
 public:
  explicit Fd(int value = -1) noexcept : value_(value) {}
  ~Fd() { if (value_ >= 0) ::close(value_); }
  Fd(const Fd&) = delete;
  Fd& operator=(const Fd&) = delete;
  Fd(Fd&& other) noexcept : value_(other.release()) {}
  int get() const noexcept { return value_; }
  int release() noexcept { return std::exchange(value_, -1); }
 private:
  int value_;
};
class Mapping {
 public:
  Mapping(void* address, std::size_t bytes) : address_(address), bytes_(bytes) {
    if (address == MAP_FAILED) fail("mmap");
  }
  ~Mapping() { ::munmap(address_, bytes_); }
  Mapping(const Mapping&) = delete;
  Mapping& operator=(const Mapping&) = delete;
  void* get() const noexcept { return address_; }
 private:
  void* address_;
  std::size_t bytes_;
};
class Child {
 public:
  explicit Child(pid_t pid) : pid_(pid) {}
  ~Child() {
    if (pid_ > 0) {
      ::kill(pid_, SIGKILL);
      while (::waitpid(pid_, nullptr, 0) < 0 && errno == EINTR) {}
    }
  }
  Child(const Child&) = delete;
  Child& operator=(const Child&) = delete;
  int join() {
    int status = 0;
    pid_t rc;
    do { rc = ::waitpid(pid_, &status, 0); } while (rc < 0 && errno == EINTR);
    if (rc < 0) fail("waitpid");
    pid_ = -1;
    return status;
  }
 private:
  pid_t pid_;
};
}  // namespace lab::support
namespace lab {
struct ProcessStats { std::uint64_t rss_bytes, vm_bytes, threads; };
// Прочитать VmRSS, VmSize (kB=1024 bytes), Threads из файла формата /proc/PID/status.
// Порядок произвольный, неизвестные строки игнорировать. Все 3 поля обязательны.
// Дубли, неверное число/единица/переполнение -> invalid_argument.
// Threads >= 1. Ошибка open/read -> system_error; fd закрывается всегда.
ProcessStats read_process_status(const char* path);
}
