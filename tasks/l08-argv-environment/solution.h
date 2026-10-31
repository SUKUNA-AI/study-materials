#pragma once
#include <string>
namespace lab {
struct Config { unsigned threads; std::string label; };
// argv[0] — имя программы, не опция; argc>=1, все argv[i] — NUL-terminated.
// Разрешены --threads N и --label TEXT в любом порядке, каждое не более одного раза.
// Приоритет для КАЖДОГО поля: аргумент > переменная STUDY_THREADS/STUDY_LABEL >
// значение по умолчанию 1/"run". Неиспользуемое окружение не проверять.
// N — непустая строка десятичных цифр, значение 1..64; ведущие нули разрешены.
// Пустая метка допустима и отличается от отсутствующей.
// Неизвестная/повторная опция, отсутствующее значение, плохое N -> invalid_argument.
// Вернуть СВОЮ строку, не указатель getenv. Никаких shell-разбиений по пробелам.
Config parse_config(int argc,const char* const* argv);
}
