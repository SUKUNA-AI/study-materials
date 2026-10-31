#include "../solution.h"
#include "helpers.h"
#include <sys/wait.h>
#include <unistd.h>

// Public property probe: observes the calling process. No process_identity body.
int main() {
  try {
    const auto in=jio::read_input();
    const int repeats=number(in,"repeats");
    auto matches=[&]() {
      for(int i=0;i<repeats;++i) {
        const auto value=process_identity();
        if(value.pid!=::getpid() || value.parent!=::getppid() ||
           value.real_uid!=::getuid() || value.effective_uid!=::geteuid()) return false;
      }
      return true;
    };
    bool ok=false;
    if(in.at("child").as_bool()) {
      const bool parent_ok=matches(); // Warm lazy caches before the child inherits them.
      const pid_t child=::fork();
      if(child<0) return 3;
      if(child==0) ::_exit(matches()?0:7);
      int status=0;
      if(::waitpid(child,&status,0)!=child) return 3;
      ok=parent_ok && WIFEXITED(status) && WEXITSTATUS(status)==0;
    } else ok=matches();
    auto out=jio::Value::object();out.set("matches_process",ok);jio::emit(out);
    return 0;
  } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 3;}
}
