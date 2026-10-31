#include "../solution.h"
#include "support.h"
namespace {
timespec points[2];int clock_calls=0,work_calls=0,fail_at=-1;bool monotonic=true;
void work(){++work_calls;}
int fake_clock(clockid_t id,timespec* t) {
  monotonic=monotonic && id==CLOCK_MONOTONIC;
  const int i=clock_calls++;
  if(i==fail_at){errno=EINVAL;return -1;}
  if(i>1){errno=EIO;return -1;}
  *t=points[i];return 0;
}
timespec point(const Value& v){return {static_cast<time_t>(v.at(0).as_int()),static_cast<long>(v.at(1).as_int())};}
}
int main() {
  try {
    const auto in=jio::read_input();auto out=Value::object();
    const auto mode=in.get_str("mode","difference");
    try {
      if(mode=="difference"){
        out.set("ns",lab::difference_ns(point(in.at("start")),point(in.at("end"))));
      }else if(mode=="real"){
        out.set("nonnegative",lab::measure_ns(work)>=0).set("work_calls",work_calls);
      }else{
        points[0]=point(in.at("start"));points[1]=point(in.at("end"));
        fail_at=static_cast<int>(in.get_int("fail_at",-1));
        out.set("ns",lab::measure_ns(work,fake_clock));
      }
    }catch(const std::system_error& e){out=fixture::error_value(e);}
     catch(const std::invalid_argument&){out.set("error","invalid_argument");}
    if(mode=="measure")out.set("clock_calls",clock_calls).set("work_calls",work_calls).set("monotonic",monotonic);
    jio::emit(out);
  }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 2;}
}
