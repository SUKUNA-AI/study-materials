#include "../solution.h"
#include "support.h"
#include <cstdio>
Value run(const Value& in) {
  auto out=Value::object();
  if(in.at("case").as_str()=="live") {
    const auto stats=lab::read_process_status("/proc/self/status");
    out.set("valid",stats.threads>=1 && stats.vm_bytes>0);return out;
  }
  FILE* file=::tmpfile();fixture::check(file!=nullptr,"tmpfile");
  const auto content=in.at("text").as_str();
  if(::fwrite(content.data(),1,content.size(),file)!=content.size() || ::fflush(file)!=0){::fclose(file);throw std::runtime_error("fixture write");}
  const std::string path="/proc/self/fd/"+std::to_string(::fileno(file));
  try {
    const auto stats=lab::read_process_status(path.c_str());
    out.set("rss_bytes",stats.rss_bytes).set("vm_bytes",stats.vm_bytes).set("threads",stats.threads);
  } catch(const std::invalid_argument&){out.set("invalid",true);}
    catch(...){::fclose(file);throw;}
  ::fclose(file);return out;
}
int main() {
  auto output=Value::object();
  try {
    const auto input=jio::read_input();const int before=fixture::fd_count();
    output.set("result",run(input));output.set("fd_leak",fixture::fd_count()-before);
  } catch(const std::exception& error){output.set("exception",error.what());}
  jio::emit(output);return 0;
}
