#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    out = nullable(quotient(number(in,"a"),number(in,"b")));
    jio::Value wrapped = jio::Value::object();
    wrapped.set("value", out);
    jio::emit(wrapped);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
