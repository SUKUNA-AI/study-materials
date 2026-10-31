#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto v=ints(in.at("values")); auto p=v.empty()?nullptr:std::make_unique<int[]>(v.size());
 for(std::size_t i=0;i<v.size();++i) { p[i]=v[i]; }
 const int* before=p.get();
 int* raw=hand_out(p); bool released=!p, same=raw==before;
 std::unique_ptr<int[]> receiver;
 if(released && same) receiver.reset(raw);
 auto values=jio::Value::array(); if(receiver) for(std::size_t i=0;i<v.size();++i) values.push(receiver[i]);
 out=jio::Value::object(); out.set("released",released).set("same_block",same).set("values",values);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
