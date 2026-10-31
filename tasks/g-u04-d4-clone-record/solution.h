#pragma once
#include <memory>
#include <string>
#include <cstddef>
struct Record { std::string name; std::unique_ptr<int[]> values; std::size_t count; };
std::unique_ptr<Record> clone_record(const Record& source);
