#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    out=join_pair(in.at("left").as_str(),in.at("right").as_str());
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
