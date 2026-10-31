#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto a=make_cell(number(in,"value")); auto b=make_cell(number(in,"value"));
 out=jio::Value::object(); out.set("two_owners",bool(a)&&bool(b)).set("independent",a.get()!=b.get());
 if(a && b) { out.set("value",*a); *b=17; out.set("unchanged",*a==number(in,"value")); }
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
