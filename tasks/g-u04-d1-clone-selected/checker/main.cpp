#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::vector<std::unique_ptr<int>> src; for(const auto& x:in.at("values").items()) src.push_back(x.is_null()?nullptr:std::make_unique<int>(static_cast<int>(x.as_int())));
 auto r=clone_positive(src);out=jio::Value::object();auto vals=jio::Value::array();bool independent=true;
 for(const auto& p:r){ if(!p) {independent=false;vals.push(jio::Value());continue;} vals.push(*p);for(const auto& q:src)if(q.get()==p.get()) independent=false; }
 for(auto& q:src) if(q) *q=0;
 auto after=jio::Value::array();for(const auto& p:r) after.push(p?jio::Value(*p):jio::Value());
 out.set("values",vals).set("after",after).set("independent",independent);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
