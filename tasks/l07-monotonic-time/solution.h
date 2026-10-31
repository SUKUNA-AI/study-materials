#pragma once
#include <cstdint>
#include <time.h>
namespace lab {
using ClockFn=int(*)(clockid_t,timespec*);
// Секунды в [0,1e9], наносекунды в [0,1e9); end>=start.
// Нарушение этих ограничений -> invalid_argument. Результат помещается в int64_t.
std::int64_t difference_ns(timespec start,timespec end);
// Два clock_fn(CLOCK_MONOTONIC): до и после однократного work().
// Неудача clock_fn -> system_error с немедленно сохранённым errno.
// work не бросает. Если первый отсчёт не получен, work не вызывается.
std::int64_t measure_ns(void(*work)(),ClockFn clock_fn=::clock_gettime);
}
