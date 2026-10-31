#pragma once
#include <cstddef>
#include <string>
#include <sys/types.h>
#include <unistd.h>
namespace lab {
using ReadFn = ssize_t (*)(int, void*, std::size_t);
enum class ReadState { data, eof, retry, would_block, error };
struct ReadResult { ReadState state; std::string bytes; int error; };
// capacity <=4096; для capacity==0 вернуть data,"",0 без системного вызова.
// fd заимствован: не закрывать. При capacity>0 вызвать read_fn ОДИН раз.
// n>0: data с ровно n байтами, error=0, даже если errno ненулевой.
// n==0: eof,"",0. n==-1: EINTR -> retry, EAGAIN/EWOULDBLOCK -> would_block,
// остальные -> error. В этих трёх случаях bytes="", error=сохранённый errno.
ReadResult attempt_read(int fd, std::size_t capacity, ReadFn read_fn = ::read);
}
