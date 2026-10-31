#pragma once
#include <string>
enum class State : unsigned { queued=0, running=1, done=2 };
std::string state_name(State state);
