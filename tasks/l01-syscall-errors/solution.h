// solution.h — задача l01-syscall-errors (L1. Керриск гл. 2–3: системные вызовы, errno, std::system_error).
// Даёт судья; ученик сдаёт solution.cpp с определениями трёх функций.
#pragma once

#include <cstdint>

namespace lab::sys {

// POSIX-соглашение: системный вызов сообщает об ошибке значением -1 и кодом в errno.
// ret == -1  -> бросить std::system_error(errno, std::generic_category(), what);
// иначе      -> вернуть ret без изменений (любое другое значение, даже отрицательное, — не ошибка).
long check(long ret, const char* what);

// Соглашение pthread_*/posix_spawn: функция ВОЗВРАЩАЕТ код ошибки (0 — успех) и errno НЕ трогает.
// rc != 0 -> бросить std::system_error(rc, std::generic_category(), what).
void check_rc(int rc, const char* what);

// Размер обычного файла в байтах: open(path, O_RDONLY | O_CLOEXEC) -> fstat -> close.
// Ошибка open/fstat -> std::system_error с errno этого вызова; не обычный файл (каталог, устройство…)
// -> std::system_error с кодом EISDIR для каталога и EINVAL для прочего. В what() должен быть path.
// Дескриптор закрывается на ЛЮБОМ пути, в том числе при исключении.
std::int64_t file_size(const char* path);

}  // namespace lab::sys
