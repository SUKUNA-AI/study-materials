#include "../solution.h"
#include "helpers.h"
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

// Inject exactly one short/error operation; retry/accumulation belongs to the student.
namespace public_probe {
std::size_t cap=4096;bool interrupt=false;bool hard_error=false;
ssize_t read_at(int fd,void* data,std::size_t count,off_t offset) {
  if(interrupt){interrupt=false;errno=EINTR;return -1;}
  if(hard_error){errno=EIO;return -1;}
  errno=EINVAL;
  return ::pread(fd,data,count<cap?count:cap,offset);
}
}
int main() {
  FILE* fixture=nullptr;
  try {
    const auto in=jio::read_input();const auto bytes=in.at("bytes").as_str();
    fixture=::tmpfile();if(!fixture)return 3;
    if(::fwrite(bytes.data(),1,bytes.size(),fixture)!=bytes.size() || ::fflush(fixture)!=0){::fclose(fixture);return 3;}
    const int fd=::fileno(fixture);if(::lseek(fd,2,SEEK_SET)!=2){::fclose(fixture);return 3;}
    public_probe::cap=static_cast<std::size_t>(in.get_int("chunk",4096));
    public_probe::interrupt=in.get_bool("eintr",false);public_probe::hard_error=in.get_bool("error",false);
    auto out=jio::Value::object();
    try {
      out.set("bytes",read_slice(fd,static_cast<off_t>(in.at("offset").as_int()),
        static_cast<std::size_t>(in.at("count").as_int()),public_probe::read_at)).set("error",false);
    } catch(const std::system_error& error){
      out.set("error",error.code().value()==EIO).set("generic",error.code().category()==std::generic_category());
    }
    out.set("position_preserved",::lseek(fd,0,SEEK_CUR)==2).set("borrowed_open",::fcntl(fd,F_GETFD)>=0);
    ::fclose(fixture);fixture=nullptr;jio::emit(out);return 0;
  } catch(const std::exception& error){if(fixture)::fclose(fixture);std::cerr<<error.what()<<'\n';return 3;}
}
