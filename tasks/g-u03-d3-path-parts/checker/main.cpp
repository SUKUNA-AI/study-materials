#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    const std::string text=in.at("text").as_str();auto r=path_parts(text);out=jio::Value::array();for(auto p:r){auto x=jio::Value::object();x.set("part",std::string(p)).set("offset",offset_of(text,p));out.push(x);}
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
