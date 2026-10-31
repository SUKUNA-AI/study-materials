#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::vector<Sample> v; for(const auto& a:in.at("samples").items()) v.push_back({number(a,"id"),number(a,"value"),a.at("valid").as_bool()}); out=nullable(best_sample(v));
    jio::Value wrapped = jio::Value::object();
    wrapped.set("value", out);
    jio::emit(wrapped);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
