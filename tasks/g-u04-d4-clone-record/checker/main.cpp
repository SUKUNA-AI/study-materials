#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    auto v=ints(in.at("values"));Record src{in.at("name").as_str(),v.empty()?nullptr:std::make_unique<int[]>(v.size()),v.size()};for(std::size_t i=0;i<v.size();++i){src.values[i]=v[i];}
 auto r=clone_record(src);out=jio::Value::object();if(!r){out.set("missing",true);}else{auto vals=jio::Value::array();bool independent=r->count==0?r->values==nullptr:r->values.get()!=src.values.get();src.name="changed";for(std::size_t i=0;i<v.size();++i){src.values[i]=99;}
 if(r->values){for(std::size_t i=0;i<r->count;++i){vals.push(r->values[i]);}}
 out.set("name",r->name).set("values",vals).set("count",r->count).set("independent",independent);}
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
