#pragma once
#include <cstdint>
struct Account { int balance; int held; };
bool reserve_amount(Account& account, int amount);
