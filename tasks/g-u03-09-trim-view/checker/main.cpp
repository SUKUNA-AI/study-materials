#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    const auto& owner=in.at("text").as_str(); auto view=trim_ascii(owner);
 out=jio::Value::object(); out.set("text",std::string(view)).set("offset",offset_of(owner,view));
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
