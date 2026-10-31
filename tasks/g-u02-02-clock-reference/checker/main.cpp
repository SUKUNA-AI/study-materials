#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    int h=number(in,"hour"), m=number(in,"minute"); advance_clock(h,m,number(in,"delta")); out = pair_value(h,m);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
