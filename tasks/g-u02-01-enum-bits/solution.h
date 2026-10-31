#pragma once
enum class Access : unsigned { read=1, write=2, execute=4 };
unsigned access_bits(Access value);
