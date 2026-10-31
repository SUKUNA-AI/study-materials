#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    out=jio::Value::object();
 { auto p=in.at("parent").as_bool()?std::make_shared<Node>():nullptr;
   auto c=in.at("child").as_bool()?std::make_shared<Node>():nullptr;
   link_nodes(p,c);
   out.set("forward",p && c && p->child==c).set("backward",p && c && c->parent.lock()==p);
 }
 out.set("live_after",Node::live);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
