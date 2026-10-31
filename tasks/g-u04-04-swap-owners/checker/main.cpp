#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto av=maybe_int(in.at("a")), bv=maybe_int(in.at("b"));
 auto a=av?std::make_unique<int>(*av):nullptr, b=bv?std::make_unique<int>(*bv):nullptr;
 const int* ap=a.get(); const int* bp=b.get(); bool same=in.at("same").as_bool();
 if(same) swap_cells(a,a); else swap_cells(a,b);
 out=jio::Value::object(); out.set("a",a?jio::Value(*a):jio::Value()).set("b",b?jio::Value(*b):jio::Value());
 out.set("same_blocks",same?(a.get()==ap && b.get()==bp):(a.get()==bp && b.get()==ap));
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
