#pragma once
#include "judge_io.h"
#include <optional>
#include <string_view>
#include <vector>
inline int number(const jio::Value& v, const char* key) {
  return static_cast<int>(v.at(key).as_int());
}
inline std::vector<int> ints(const jio::Value& v) {
  std::vector<int> r;
  for (const auto& x : v.items()) r.push_back(static_cast<int>(x.as_int()));
  return r;
}
template<class T> jio::Value array(const std::vector<T>& v) {
  auto r = jio::Value::array();
  for (const auto& x : v) r.push(x);
  return r;
}
inline jio::Value pair_value(int a, int b) {
  auto r = jio::Value::array(); r.push(a); r.push(b); return r;
}
inline std::optional<int> maybe_int(const jio::Value& v) {
  if (v.is_null()) return std::nullopt;
  return static_cast<int>(v.as_int());
}
template<class T> jio::Value nullable(const std::optional<T>& v) {
  return v ? jio::Value(*v) : jio::Value();
}
inline long long offset_of(const std::string& owner, std::string_view view) {
  // Не вычитаем указатели на разные объекты. Проверяем принадлежность и границы.
  for (std::size_t i = 0; i <= owner.size(); ++i) {
    if (view.data() == owner.data() + i && view.size() <= owner.size() - i)
      return static_cast<long long>(i);
  }
  return -1;
}
