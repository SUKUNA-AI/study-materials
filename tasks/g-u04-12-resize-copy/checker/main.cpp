#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto v=ints(in.at("values")); auto p=v.empty()?nullptr:std::make_unique<int[]>(v.size());
 for(std::size_t i=0;i<v.size();++i) { p[i]=v[i]; }
 const int* before=p.get();
 auto n=static_cast<std::size_t>(number(in,"new_size")); auto result=resize_copy(p,v.size(),n);
 auto values=jio::Value::array(); if(result) for(std::size_t i=0;i<n;++i) values.push(result[i]);
 bool preserved=p.get()==before; for(std::size_t i=0;i<v.size();++i) preserved=preserved && p[i]==v[i];
 out=jio::Value::object(); out.set("empty",!result).set("values",values).set("preserved",preserved);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
