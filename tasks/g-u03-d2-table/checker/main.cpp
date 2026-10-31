#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    std::vector<std::pair<std::string,int>> rows; for(const auto& r:in.at("rows").items())rows.emplace_back(r.at("name").as_str(),number(r,"value")); out=render_table(rows);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
