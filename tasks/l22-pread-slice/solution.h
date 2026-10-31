#pragma once
#include <cstddef>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <system_error>
using PreadFn=ssize_t(*)(int,void*,std::size_t,off_t);
std::string read_slice(int fd, off_t offset, std::size_t count, PreadFn rd);
