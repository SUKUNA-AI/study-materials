#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    int v=number(in,"value"); bool ok=try_delta(v,number(in,"delta"),number(in,"cap")); out=jio::Value::object(); out.set("ok",ok).set("value",v);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
