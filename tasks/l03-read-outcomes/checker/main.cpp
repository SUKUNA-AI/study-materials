#include "../solution.h"
#include "support.h"
namespace {
int calls=0, injected=0;
ssize_t interrupted(int,void*,std::size_t) { ++calls; errno=injected; return -1; }
ssize_t counted_read(int f,void* p,std::size_t n) { ++calls;return ::read(f,p,n); }
std::string state(lab::ReadState s) {
  switch(s) {
    case lab::ReadState::data:return "data";
    case lab::ReadState::eof:return "eof";
    case lab::ReadState::retry:return "retry";
    case lab::ReadState::would_block:return "would_block";
    case lab::ReadState::error:return "error";
  }
  return "invalid";
}
}
int main() {
  int pipes[2]={-1,-1};
  auto cleanup=[&](){for(int& fd:pipes)if(fd>=0){::close(fd);fd=-1;}};
  try {
    const auto in=jio::read_input();
    fixture::check(::pipe(pipes)==0,"pipe");
    int& reader=pipes[0];int& writer=pipes[1];
    const auto data=in.get_str("data","");
    if (!data.empty()) fixture::check(::write(writer,data.data(),data.size())==static_cast<ssize_t>(data.size()),"write fixture");
    if (in.get_bool("eof",false)) {::close(writer);writer=-1;}
    fixture::check(::fcntl(reader,F_SETFL,O_NONBLOCK)==0,"fcntl");
    lab::ReadFn fn=counted_read;
    const auto code=in.get_str("inject","");
    if (!code.empty()) {
      injected=code=="EINTR"?EINTR:code=="EIO"?EIO:EBADF;
      fn=interrupted;
    }
    errno=EIO;
    auto got=lab::attempt_read(reader,static_cast<std::size_t>(in.get_int("capacity",16)),fn);
    auto out=Value::object();
    out.set("state",state(got.state)).set("bytes",got.bytes).set("error",fixture::error_name(got.error))
       .set("calls",calls).set("borrowed_open",::fcntl(reader,F_GETFD)!=-1);
    cleanup();jio::emit(out);
  } catch(const std::exception& e){cleanup();std::cerr<<e.what()<<'\n';return 2;}
}
