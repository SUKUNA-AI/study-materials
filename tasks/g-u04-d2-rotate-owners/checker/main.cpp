#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::array<std::unique_ptr<int>,3> a;std::array<const int*,3> old{};for(std::size_t i=0;i<3;++i){if(!in.at("values").at(i).is_null())a[i]=std::make_unique<int>(static_cast<int>(in.at("values").at(i).as_int()));old[i]=a[i].get();}
 rotate_owners(a);out=jio::Value::object();auto v=jio::Value::array();for(auto& p:a)v.push(p?jio::Value(*p):jio::Value());out.set("values",v).set("same_objects",a[0].get()==old[2]&&a[1].get()==old[0]&&a[2].get()==old[1]);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
