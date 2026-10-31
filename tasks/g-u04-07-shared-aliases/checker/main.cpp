#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto v=maybe_int(in.at("value")); auto p=v?std::make_shared<int>(*v):nullptr;
 auto pair=share_twice(p); auto before=p.get(); auto count=p.use_count(); p.reset();
 out=jio::Value::object(); out.set("same_object",pair[0].get()==before && pair[1].get()==before)
    .set("initial_owners",count);
 if(pair[0]) *pair[0]=number(in,"replacement");
 out.set("observed",pair[1]?jio::Value(*pair[1]):jio::Value());
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
