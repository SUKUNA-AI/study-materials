#pragma once
#include <cstddef>
#include <sys/types.h>
#include <unistd.h>
namespace lab {
struct IoOps {
  ssize_t (*read_fn)(int, void*, std::size_t) = ::read;
  ssize_t (*write_fn)(int, const void*, std::size_t) = ::write;
};
// Два разных обычных файла. Источник <=65536 байт. Получатель создаётся или обрезается.
// Копировать до EOF через операции ops: короткий успех — прогресс, EINTR — повтор.
// Ошибка, включая write(..., n>0)==0: system_error(generic_category), для нуля EIO.
// Вернуть число скопированных байтов; закрыть оба открытых дескриптора на любом пути.
// Ошибка close после успешной записи не проверяется: это задача о переносе байтов,
// а не о гарантии сохранения данных. На Linux close в деструкторе не повторять.
std::size_t copy_file(const char* source, const char* destination, IoOps ops = {});
}
