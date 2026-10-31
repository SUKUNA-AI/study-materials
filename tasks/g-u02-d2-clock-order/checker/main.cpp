#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    out=compare_clock(Clock{number(in,"ah"),number(in,"am")},Clock{number(in,"bh"),number(in,"bm")});
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
