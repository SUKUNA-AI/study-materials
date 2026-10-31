#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::weak_ptr<int> w; std::shared_ptr<int> p;
 if(!in.at("value").is_null()) { p=std::make_shared<int>(number(in,"value")); w=p; }
 if(in.at("expired").as_bool()) p.reset();
 out=nullable(weak_value(w));
    jio::Value wrapped = jio::Value::object();
    wrapped.set("value", out);
    jio::emit(wrapped);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
