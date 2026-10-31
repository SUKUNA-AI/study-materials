#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::string text=in.at("text").as_str(); auto r=parse_assignment(text); text.assign(text.size(),'X'); if(r){out=jio::Value::array();out.push(r->first).push(r->second);}else out=jio::Value();
    jio::Value wrapped = jio::Value::object();
    wrapped.set("value", out);
    jio::emit(wrapped);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
