#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto current=maybe_int(in.at("current")); bool changed=replace_present(current,maybe_int(in.at("incoming"))); out=jio::Value::object(); out.set("changed",changed).set("value",nullable(current));
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
