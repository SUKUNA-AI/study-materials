#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::vector<std::shared_ptr<int>> owners;std::vector<std::weak_ptr<int>> refs;
 for(const auto& v:in.at("values").items()){auto p=std::make_shared<int>(static_cast<int>(v.at("value").as_int()));refs.push_back(p);if(v.at("alive").as_bool())owners.push_back(p);}
 auto result=sum_live(refs);bool unchanged=true;for(auto& p:owners)unchanged=unchanged&&p.use_count()==1;out=jio::Value::object();out.set("sum",result).set("ownership_unchanged",unchanged);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
