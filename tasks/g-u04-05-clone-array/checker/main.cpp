#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto v=ints(in.at("values")); auto p=clone_values(v.empty()?nullptr:v.data(),v.size());
 for(auto& x:v) x=123456;
 auto values=jio::Value::array(); if(p) for(std::size_t i=0;i<v.size();++i) values.push(p[i]);
 out=jio::Value::object(); out.set("empty",!p).set("values",values);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
