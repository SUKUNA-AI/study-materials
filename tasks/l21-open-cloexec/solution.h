#pragma once
#include <cerrno>
#include <system_error>
#include <fcntl.h>
#include <unistd.h>
// Готовая передача ошибки: ученик не пишет инфраструктуру исключений.
[[noreturn]] inline void fail_open(int saved,const char* path){throw std::system_error(saved,std::generic_category(),path);}
int open_input(const char* path);
