#include "solution.h"

inline std::int64_t scratch_total(std::span<const int> v, bool cancel) {
  int* scratch = new int[v.size()];
  std::copy(v.begin(),v.end(),scratch);
  if(cancel) throw std::runtime_error("cancel");
  std::int64_t sum=0;
  for(std::size_t i=0;i<v.size();++i) sum+=scratch[i];
  delete scratch;
  return sum;
}
