#pragma once
#include "judge_io.h"
#include <algorithm>
#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
using jio::Value;
namespace fixture {
inline void check(bool ok,const char* operation) {
  if(!ok)throw std::system_error(errno,std::generic_category(),operation);
}
inline int fd_count() {
  DIR* directory=::opendir("/proc/self/fd");check(directory!=nullptr,"opendir");
  int count=-1;while(auto* entry=::readdir(directory))if(entry->d_name[0]!='.')++count;
  ::closedir(directory);return count;
}
inline std::string error_name(int code) {
  if(code==0)return "none";
  if(code==ENOENT)return "ENOENT";
  if(code==EIO)return "EIO";
  if(code==EINTR)return "EINTR";
  if(code==EAGAIN || code==EWOULDBLOCK)return "EAGAIN";
  if(code==EBADF)return "EBADF";
  if(code==EISDIR)return "EISDIR";
  if(code==ENOMEM)return "ENOMEM";
  if(code==EINVAL)return "EINVAL";
  return "other";
}
inline Value error_value(const std::system_error& error) {
  auto out=Value::object();out.set("error",error_name(error.code().value()))
    .set("generic",error.code().category()==std::generic_category());return out;
}
inline std::string pattern(std::size_t count) {
  std::string bytes(count,'\0');for(std::size_t i=0;i<count;++i)bytes[i]=static_cast<char>((i*37U+11U)%251U);return bytes;
}
// Standard stream fixture IO; student retry/read/write logic is absent here.
inline void write_file(const std::string& path,const std::string& bytes) {
  std::ofstream file(path,std::ios::binary|std::ios::trunc);check(bool(file),"fixture open");
  file.write(bytes.data(),static_cast<std::streamsize>(bytes.size()));check(bool(file),"fixture write");
}
inline std::string read_file(const std::string& path) {
  std::ifstream file(path,std::ios::binary);check(bool(file),"fixture read open");return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
}
