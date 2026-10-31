#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::vector<Sample> items; for(const auto& v:in.at("samples").items()) items.push_back({static_cast<int>(v.at(0).as_int()),v.at(1).as_bool()}); out=nullable(valid_total(items));
    jio::Value wrapped = jio::Value::object();
    wrapped.set("value", out);
    jio::emit(wrapped);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
