#include "../solution.h"
#include "helpers.h"

int main() {
  try {
    const auto in = jio::read_input();
    jio::Value out;
    Account a{number(in,"balance"),number(in,"held")}; bool ok=reserve_amount(a,number(in,"amount")); out=jio::Value::object(); out.set("ok",ok).set("balance",a.balance).set("held",a.held);
    jio::emit(out);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 3;
  }
}
