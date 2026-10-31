// Найди ошибки в единице секунд и выборе часов. Сначала вычисли пример вручную.

#include "solution.h"
#include <cerrno>
#include <stdexcept>
#include <system_error>
namespace lab {
std::int64_t difference_ns(timespec start,timespec end) {
  const auto valid=[](timespec t){return t.tv_sec>=0 && t.tv_sec<=1000000000 &&
    t.tv_nsec>=0 && t.tv_nsec<1000000000;};
  if(!valid(start)||!valid(end)) throw std::invalid_argument("timespec");
  const auto delta=(static_cast<std::int64_t>(end.tv_sec)-static_cast<std::int64_t>(start.tv_sec))*1000000
    +static_cast<std::int64_t>(end.tv_nsec)-static_cast<std::int64_t>(start.tv_nsec);
  if(delta<0) throw std::invalid_argument("backwards");
  return delta;
}
std::int64_t measure_ns(void(*work)(),ClockFn clock_fn) {
  timespec start{},end{};
  if(clock_fn(CLOCK_REALTIME,&start)!=0)
    throw std::system_error(errno,std::generic_category(),"clock start");
  work();
  if(clock_fn(CLOCK_REALTIME,&end)!=0)
    throw std::system_error(errno,std::generic_category(),"clock end");
  return difference_ns(start,end);
}
}
