#include "../solution.h"
#include "support.h"
namespace {
void set(const Value& in,const char* key,const char* env) {
  if(in.has(key))fixture::check(::setenv(env,in.at(key).as_str().c_str(),1)==0,"setenv");
  else fixture::check(::unsetenv(env)==0,"unsetenv");
}
}
int main() {
  try {
    const auto in=jio::read_input();
    set(in,"env_threads","STUDY_THREADS");set(in,"env_label","STUDY_LABEL");
    std::vector<std::string> args={"program name"};
    for(const auto& a:in.at("args").items())args.push_back(a.as_str());
    std::vector<const char*> argv;
    for(const auto& a:args)argv.push_back(a.c_str());
    argv.push_back(nullptr);
    auto out=Value::object();
    try {
      auto result=lab::parse_config(static_cast<int>(args.size()),argv.data());
      ::setenv("STUDY_LABEL","changed-after-return",1);
      out.set("threads",result.threads).set("label",result.label);
    }catch(const std::invalid_argument&){out.set("error","invalid_argument");}
    jio::emit(out);
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 2;}
}
