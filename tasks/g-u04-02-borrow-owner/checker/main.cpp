#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto value=maybe_int(in.at("value")); auto p=value?std::make_unique<int>(*value):nullptr;
 const int* before=p.get(); int result=read_or(p,number(in,"fallback"));
 out=jio::Value::object(); out.set("value",result).set("preserved",p.get()==before);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
