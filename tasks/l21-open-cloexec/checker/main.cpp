#include "../solution.h"
#include "helpers.h"
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

// Only fixture setup/observation. No open_input body and no hand-written IO loop.
int main() {
  char path[]="/tmp/study-public-open-XXXXXX";
  try {
    const auto in=jio::read_input();const auto bytes=in.at("bytes").as_str();
    const int created=::mkstemp(path);if(created<0)return 3;
    FILE* fixture=::fdopen(created,"wb");if(!fixture){::close(created);::unlink(path);return 3;}
    const bool written=::fwrite(bytes.data(),1,bytes.size(),fixture)==bytes.size();
    const bool closed=::fclose(fixture)==0;if(!written || !closed){::unlink(path);return 3;}
    if(in.get_bool("missing",false))::unlink(path);
    if(in.get_bool("fd_zero",false))::close(STDIN_FILENO);
    auto out=jio::Value::object();
    try {
      const int fd=open_input(path);::unlink(path);
      if(fd<0)return 3;
      const int flags=::fcntl(fd,F_GETFD),status=::fcntl(fd,F_GETFL);
      if(flags<0 || status<0){::close(fd);return 3;}
      FILE* observed=::fdopen(fd,"rb");if(!observed){::close(fd);return 3;}
      std::vector<char> buffer(bytes.size()+1);
      const auto size=::fread(buffer.data(),1,buffer.size(),observed);
      const bool read_ok=::ferror(observed)==0;::fclose(observed);if(!read_ok)return 3;
      out.set("missing",false).set("cloexec",(flags&FD_CLOEXEC)!=0)
        .set("read_only",(status&O_ACCMODE)==O_RDONLY)
        .set("bytes",std::string(buffer.data(),size))
        .set("zero_fd_ok",!in.get_bool("fd_zero",false)||fd==0);
    } catch(const std::system_error& error) {
      ::unlink(path);out.set("missing",error.code().value()==ENOENT)
        .set("generic",error.code().category()==std::generic_category());
    }
    jio::emit(out);return 0;
  } catch(const std::exception& error){::unlink(path);std::cerr<<error.what()<<'\n';return 3;}
}
