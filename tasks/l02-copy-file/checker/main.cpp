#include "../solution.h"
#include "support.h"
namespace {
std::size_t rcap, wcap;
int rintr, wintr, rcalls, wcalls, fail_read, fail_write;
bool zero_write;
ssize_t short_read(int fd, void* dst, std::size_t n) {
  if (++rcalls > 200000) { errno = EIO; return -1; }
  if (rintr-- > 0) { errno = EINTR; return -1; }
  if (fail_read) { errno = EIO; return -1; }
  return ::read(fd,dst,std::min(n,rcap));
}
ssize_t short_write(int fd, const void* src, std::size_t n) {
  if (++wcalls > 200000) { errno = EIO; return -1; }
  if (wintr-- > 0) { errno = EINTR; return -1; }
  if (fail_write) { errno = EIO; return -1; }
  if (zero_write) return 0;
  return ::write(fd,src,std::min(n,wcap));
}
}
int main() {
  std::string directory;
  auto cleanup=[&](){if(!directory.empty()){
    ::unlink((directory+"/in").c_str());::unlink((directory+"/out").c_str());::rmdir(directory.c_str());
  }};
  try {
    const auto in=jio::read_input();
    char pattern[]="/tmp/study-public-copy-XXXXXX";
    const char* created=::mkdtemp(pattern);fixture::check(created!=nullptr,"mkdtemp");directory=created;
    const std::string src=directory+"/in", dst=directory+"/out";
    const auto bytes=fixture::pattern(static_cast<std::size_t>(in.get_int("size",0)));
    fixture::write_file(src,bytes);
    fixture::write_file(dst,"stale destination bytes");
    rcap=static_cast<std::size_t>(in.get_int("read_cap",65536));
    wcap=static_cast<std::size_t>(in.get_int("write_cap",65536));
    rintr=static_cast<int>(in.get_int("read_eintr",0));
    wintr=static_cast<int>(in.get_int("write_eintr",0));
    fail_read=in.get_bool("fail_read",false);
    fail_write=in.get_bool("fail_write",false);
    zero_write=in.get_bool("zero_write",false);
    if (in.get_bool("missing",false)) ::unlink(src.c_str());
    const int before=fixture::fd_count();
    Value out=Value::object();
    try {
      const auto n=lab::copy_file(src.c_str(),in.get_bool("dest_dir",false)?directory.c_str():dst.c_str(),
                                  {short_read,short_write});
      out.set("copied",n).set("equal",fixture::read_file(dst)==bytes);
    } catch (const std::system_error& e) { out=fixture::error_value(e); }
    out.set("fd_delta",fixture::fd_count()-before);
    cleanup();jio::emit(out);
  } catch (const std::exception& e) { cleanup();std::cerr<<e.what()<<'\n';return 2; }
}
