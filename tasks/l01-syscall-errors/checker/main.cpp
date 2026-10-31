// Стенд судьи l01-syscall-errors (Linux). stdin: {"case": ..., ...}; stdout: одна строка JSON.
// Настоящие системные вызовы в песочнице (пользователь nobody, /tmp — tmpfs, корень только для чтения).
// Утечка дескрипторов — по числу записей в /proc/self/fd до и после вызова.
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
#include <fstream>

#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "../solution.h"
#include "judge_io.h"

using jio::Value;

namespace {

std::string errno_name(int e) {
  switch (e) {
    case 0: return "0";
    case ENOENT: return "ENOENT";
    case EBADF: return "EBADF";
    case EINVAL: return "EINVAL";
    case EISDIR: return "EISDIR";
    case ENOTDIR: return "ENOTDIR";
    case EEXIST: return "EEXIST";
    case EIO: return "EIO";
    case EAGAIN: return "EAGAIN";
    case ENOMEM: return "ENOMEM";
    case EPERM: return "EPERM";
    case EDEADLK: return "EDEADLK";
    case EACCES: return "EACCES";
    case EROFS: return "EROFS";
    default: return "errno=" + std::to_string(e);
  }
}

int open_fds() {
  int n = 0;
  if (DIR* d = ::opendir("/proc/self/fd")) {
    while (dirent* e = ::readdir(d))
      if (e->d_name[0] != '.') ++n;
    ::closedir(d);
    --n;  // сам opendir
  }
  return n;
}

// Что бросила функция: {"errno": имя, "category": generic|system|other, "what_has": bool}.
template <typename F>
Value outcome(F&& f, const std::string& must_contain) {
  Value v = Value::object();
  try {
    long r = f();
    v.set("returned", r);
  } catch (const std::system_error& e) {
    v.set("errno", errno_name(e.code().value()));
    v.set("category", e.code().category() == std::generic_category()  ? "generic"
                      : e.code().category() == std::system_category() ? "system"
                                                                      : "other");
    v.set("what_has", std::string(e.what()).find(must_contain) != std::string::npos);
  } catch (const std::exception& e) {
    v.set("other_exception", e.what());
  }
  return v;
}

long scenario(const std::string& s) {
  using lab::sys::check;
  const std::string label = "L1:" + s;
  if (s == "open_missing") return check(::open("/tmp/nope/missing.bin", O_RDONLY), label.c_str());
  if (s == "close_bad") return check(::close(-1), label.c_str());
  if (s == "read_bad") {
    char b[4];
    return check(::read(12345, b, sizeof b), label.c_str());
  }
  if (s == "lseek_bad_whence") {
    int fd = ::open("/proc/self/stat", O_RDONLY | O_CLOEXEC);
    errno = 0;
    long r = ::lseek(fd, 0, 12345);
    int saved = errno;
    ::close(fd);
    errno = saved;
    return check(r, label.c_str());
  }
  if (s == "open_dir_write") return check(::open("/tmp", O_WRONLY), label.c_str());
  if (s == "open_notdir") return check(::open("/proc/self/stat/x", O_RDONLY), label.c_str());
  if (s == "open_excl_exists") {
    int fd = ::open("/tmp/l1_exists.txt", O_CREAT | O_WRONLY | O_CLOEXEC, 0600);
    if (fd >= 0) ::close(fd);
    return check(::open("/tmp/l1_exists.txt", O_CREAT | O_EXCL | O_WRONLY, 0600), label.c_str());
  }
  if (s == "dup_bad") return check(::dup(-7), label.c_str());
  if (s == "errno_preset_eio") {
    errno = EIO;
    return check(-1, label.c_str());
  }
  throw std::runtime_error("неизвестный сценарий " + s);
}

void make_file(const std::string& path,long long size) {
  std::ofstream file(path,std::ios::binary|std::ios::trunc);
  if(!file)throw std::runtime_error("fixture create");
  std::string bytes(static_cast<std::size_t>(size),'z');
  file.write(bytes.data(),static_cast<std::streamsize>(bytes.size()));
  if(!file)throw std::runtime_error("fixture write");
}

Value run(const std::string& c, const Value& in) {
  Value o = Value::object();
  if (c == "check_ok") {
    Value r = Value::array();
    errno = 0;
    for (long long v : in.int_list("values")) r.push(outcome([&] { return lab::sys::check(static_cast<long>(v), "ok"); }, "ok"));
    o.set("results", r);
    o.set("errno_untouched", errno == 0);
  } else if (c == "check_errno") {
    Value r = Value::array();
    for (const auto& s : in.at("scenarios").items()) {
      const int before = open_fds();
      r.push(outcome([&] { return scenario(s.as_str()); }, "L1:" + s.as_str()));
      ::unlink("/tmp/l1_exists.txt");
      if (open_fds() != before) o.set("fd_leak_in_harness", s.as_str());
    }
    o.set("results", r);
  } else if (c == "check_rc") {
    Value r = Value::array();
    for (long long rc : in.int_list("codes")) {
      errno = 0;
      Value v = outcome([&] { lab::sys::check_rc(static_cast<int>(rc), "rc"); return 0L; }, "rc");
      v.set("errno_untouched", errno == 0);
      r.push(v);
    }
    o.set("results", r);
    // настоящий pthread-вызов: unlock чужого/незахваченного мьютекса с проверкой ошибок -> EPERM
    pthread_mutexattr_t at;
    pthread_mutexattr_init(&at);
    pthread_mutexattr_settype(&at, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_t m;
    pthread_mutex_init(&m, &at);
    errno = 0;
    o.set("real_pthread", outcome([&] { lab::sys::check_rc(pthread_mutex_unlock(&m), "unlock"); return 0L; }, "unlock"));
    pthread_mutex_lock(&m);  // повторный lock ERRORCHECK-мьютекса тем же потоком -> EDEADLK
    o.set("relock", outcome([&] { lab::sys::check_rc(pthread_mutex_lock(&m), "relock"); return 0L; }, "relock"));
    pthread_mutex_unlock(&m);
    pthread_mutex_destroy(&m);
    pthread_mutexattr_destroy(&at);
  } else if (c == "file_size") {
    Value r = Value::array();
    int k = 0;
    for (long long size : in.int_list("sizes")) {
      const std::string p = "/tmp/l1_size_" + std::to_string(k++) + ".bin";
      make_file(p, size);
      const int before = open_fds();
      Value v = outcome([&] { return static_cast<long>(lab::sys::file_size(p.c_str())); }, p);
      v.set("fd_leak", open_fds() - before);
      r.push(v);
      ::unlink(p.c_str());
    }
    o.set("results", r);
  } else if (c == "file_size_errors") {
    Value r = Value::array();
    for (const auto& pv : in.at("paths").items()) {
      const std::string p = pv.as_str();
      const int before = open_fds();
      Value v = outcome([&] { return static_cast<long>(lab::sys::file_size(p.c_str())); }, p);
      v.set("fd_leak", open_fds() - before);
      r.push(v);
    }
    o.set("results", r);
  } else if (c == "file_size_many") {  // 300 вызовов при ulimit nofile=256: утечка = EMFILE
    make_file("/tmp/l1_many.bin", 1234);
    long long ok = 0;
    std::string err;
    for (int i = 0; i < 300; ++i) {
      try {
        lab::sys::file_size(i % 2 ? "/tmp/l1_many.bin" : "/tmp");
      } catch (const std::system_error& e) {
        if (e.code().value() != EISDIR && err.empty()) err = errno_name(e.code().value());
        if (e.code().value() == EISDIR) ++ok;
        continue;
      }
      ++ok;
    }
    ::unlink("/tmp/l1_many.bin");
    o.set("completed", ok);
    if (!err.empty()) o.set("unexpected_errno", err);
  } else {
    throw std::runtime_error("неизвестный case: " + c);
  }
  return o;
}

}  // namespace

int main() {
  Value out = Value::object();
  try {
    Value in = jio::read_input();
    out.set("result", run(in.at("case").as_str(), in));
  } catch (const std::exception& e) {
    out.set("exception", e.what());
  }
  jio::emit(out);
  return 0;
}
