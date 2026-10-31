#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto n=static_cast<std::size_t>(number(in,"count")); auto p=make_ramp(n,number(in,"start"));
 auto values=jio::Value::array(); if(p) for(std::size_t i=0;i<n;++i) values.push(p[i]);
 out=jio::Value::object(); out.set("empty",!p).set("values",values);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
