#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto value=maybe_int(in.at("value")); auto p=value?std::make_unique<Cell>(*value):nullptr;
 clear_cell(p); clear_cell(p);
 out=jio::Value::object(); out.set("empty",!p).set("live",Cell::live);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
