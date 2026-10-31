#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto old=maybe_int(in.at("old")); auto p=old?std::make_unique<Cell>(*old):nullptr;
 int made=Cell::made, gone=Cell::destroyed;
 replace_cell(p,number(in,"value")); out=jio::Value::object();
 out.set("value",p?jio::Value(p->value):jio::Value()).set("live",Cell::live)
    .set("made",Cell::made-made).set("destroyed",Cell::destroyed-gone);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
