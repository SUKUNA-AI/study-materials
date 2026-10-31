// judge_io.h — минимальный JSON для стендов судьи (harness-режим, JUDGE_SPEC §2).
// Стенд читает со stdin JSON-значение поля `input` теста и печатает ОДНУ строку JSON с результатом.
// Сравнение с `expected` делает воркер, поэтому здесь только разбор и печать.
//
// Один заголовок без зависимостей: собирается g++-14 (-std=c++23 -pedantic-errors -Wall -Wextra),
// MSVC (/W4) и nvcc 13.4 (с MSVC — в режиме C++20, см. tasks/README.md). Мастер-копия —
// tasks/_tools/judge_io.h; в каждый пакет кладётся копия (пакет самодостаточен), сверка — validate_all.py.
#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace jio {

class Value {
 public:
  enum class Type { Null, Bool, Int, Num, Str, Arr, Obj };

  Value() = default;
  Value(std::nullptr_t) {}
  Value(bool b) : t_(Type::Bool), b_(b) {}
  Value(int v) : t_(Type::Int), i_(v) {}
  Value(long v) : t_(Type::Int), i_(v) {}
  Value(long long v) : t_(Type::Int), i_(v) {}
  Value(unsigned v) : t_(Type::Int), i_(static_cast<long long>(v)) {}
  Value(unsigned long v) : t_(Type::Int), i_(static_cast<long long>(v)) {}
  Value(unsigned long long v) : t_(Type::Int), i_(static_cast<long long>(v)) {}
  Value(double v) : t_(Type::Num), d_(v) {}
  Value(float v) : t_(Type::Num), d_(static_cast<double>(v)) {}
  Value(const char* s) : t_(Type::Str), s_(s) {}
  Value(std::string s) : t_(Type::Str), s_(std::move(s)) {}

  static Value array() {
    Value v;
    v.t_ = Type::Arr;
    return v;
  }
  static Value object() {
    Value v;
    v.t_ = Type::Obj;
    return v;
  }

  Type type() const { return t_; }
  bool is_null() const { return t_ == Type::Null; }
  bool is_num() const { return t_ == Type::Int || t_ == Type::Num; }

  // ---------- чтение ----------
  const Value* find(const std::string& key) const {
    if (t_ != Type::Obj) return nullptr;
    for (const auto& kv : obj_)
      if (kv.first == key) return &kv.second;
    return nullptr;
  }
  bool has(const std::string& key) const { return find(key) != nullptr; }
  const Value& at(const std::string& key) const {
    const Value* v = find(key);
    if (!v) throw std::runtime_error("judge_io: в input нет поля '" + key + "'");
    return *v;
  }
  const Value& at(std::size_t i) const {
    if (t_ != Type::Arr || i >= arr_.size()) throw std::runtime_error("judge_io: индекс вне массива");
    return arr_[i];
  }
  std::size_t size() const { return t_ == Type::Arr ? arr_.size() : (t_ == Type::Obj ? obj_.size() : 0); }
  const std::vector<Value>& items() const { return arr_; }
  const std::vector<std::pair<std::string, Value>>& fields() const { return obj_; }

  long long as_int() const {
    if (t_ == Type::Int) return i_;
    if (t_ == Type::Num && std::floor(d_) == d_) return static_cast<long long>(d_);
    if (t_ == Type::Bool) return b_ ? 1 : 0;
    throw std::runtime_error("judge_io: ожидалось целое");
  }
  double as_double() const {
    if (t_ == Type::Num) return d_;
    if (t_ == Type::Int) return static_cast<double>(i_);
    throw std::runtime_error("judge_io: ожидалось число");
  }
  bool as_bool() const {
    if (t_ == Type::Bool) return b_;
    throw std::runtime_error("judge_io: ожидалось true/false");
  }
  const std::string& as_str() const {
    if (t_ == Type::Str) return s_;
    throw std::runtime_error("judge_io: ожидалась строка");
  }
  long long get_int(const std::string& key, long long def) const {
    const Value* v = find(key);
    return v ? v->as_int() : def;
  }
  double get_double(const std::string& key, double def) const {
    const Value* v = find(key);
    return v ? v->as_double() : def;
  }
  bool get_bool(const std::string& key, bool def) const {
    const Value* v = find(key);
    return v ? v->as_bool() : def;
  }
  std::string get_str(const std::string& key, const std::string& def) const {
    const Value* v = find(key);
    return v ? v->as_str() : def;
  }
  std::vector<long long> int_list(const std::string& key) const {
    std::vector<long long> r;
    for (const auto& x : at(key).arr_) r.push_back(x.as_int());
    return r;
  }
  std::vector<double> num_list(const std::string& key) const {
    std::vector<double> r;
    for (const auto& x : at(key).arr_) r.push_back(x.as_double());
    return r;
  }

  // ---------- построение ----------
  Value& set(const std::string& key, Value v) {
    if (t_ == Type::Null) t_ = Type::Obj;
    if (t_ != Type::Obj) throw std::logic_error("judge_io: set() не у объекта");
    for (auto& kv : obj_)
      if (kv.first == key) {
        kv.second = std::move(v);
        return *this;
      }
    obj_.emplace_back(key, std::move(v));
    return *this;
  }
  Value& push(Value v) {
    if (t_ == Type::Null) t_ = Type::Arr;
    if (t_ != Type::Arr) throw std::logic_error("judge_io: push() не у массива");
    arr_.push_back(std::move(v));
    return *this;
  }

  // ---------- печать ----------
  void dump(std::string& out) const {
    switch (t_) {
      case Type::Null: out += "null"; break;
      case Type::Bool: out += b_ ? "true" : "false"; break;
      case Type::Int: out += std::to_string(i_); break;
      case Type::Num: {
        if (!std::isfinite(d_)) {  // в JSON нет NaN/Inf — печатаем строкой
          out += std::isnan(d_) ? "\"NaN\"" : (d_ > 0 ? "\"Infinity\"" : "\"-Infinity\"");
          break;
        }
        char buf[40];
        std::snprintf(buf, sizeof buf, "%.17g", d_);
        out += buf;
        break;
      }
      case Type::Str: dump_str(out, s_); break;
      case Type::Arr: {
        out += '[';
        for (std::size_t i = 0; i < arr_.size(); ++i) {
          if (i) out += ',';
          arr_[i].dump(out);
        }
        out += ']';
        break;
      }
      case Type::Obj: {
        out += '{';
        for (std::size_t i = 0; i < obj_.size(); ++i) {
          if (i) out += ',';
          dump_str(out, obj_[i].first);
          out += ':';
          obj_[i].second.dump(out);
        }
        out += '}';
        break;
      }
    }
  }
  std::string dump() const {
    std::string s;
    dump(s);
    return s;
  }

  // ---------- разбор ----------
  static Value parse(const std::string& text) {
    std::size_t pos = 0;
    Value v = parse_value(text, pos);
    skip_ws(text, pos);
    if (pos != text.size()) throw std::runtime_error("judge_io: лишние символы после JSON");
    return v;
  }

 private:
  static void dump_str(std::string& out, const std::string& s) {
    out += '"';
    for (unsigned char c : s) {
      switch (c) {
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
          if (c < 0x20) {
            char buf[8];
            std::snprintf(buf, sizeof buf, "\\u%04x", static_cast<unsigned>(c));
            out += buf;
          } else {
            out += static_cast<char>(c);
          }
      }
    }
    out += '"';
  }
  static void skip_ws(const std::string& t, std::size_t& p) {
    while (p < t.size() && (t[p] == ' ' || t[p] == '\n' || t[p] == '\r' || t[p] == '\t')) ++p;
  }
  static void expect(const std::string& t, std::size_t& p, const char* word) {
    for (const char* w = word; *w; ++w, ++p)
      if (p >= t.size() || t[p] != *w) throw std::runtime_error("judge_io: ошибка разбора JSON");
  }
  static std::string parse_string(const std::string& t, std::size_t& p) {
    std::string r;
    ++p;  // открывающая кавычка
    while (p < t.size() && t[p] != '"') {
      char c = t[p++];
      if (c != '\\') {
        r += c;
        continue;
      }
      if (p >= t.size()) break;
      char e = t[p++];
      switch (e) {
        case 'n': r += '\n'; break;
        case 't': r += '\t'; break;
        case 'r': r += '\r'; break;
        case 'b': r += '\b'; break;
        case 'f': r += '\f'; break;
        case 'u': {  // только BMP; суррогатные пары во входах судьи не встречаются
          if (p + 4 > t.size()) throw std::runtime_error("judge_io: плохой \\u");
          unsigned cp = static_cast<unsigned>(std::stoul(t.substr(p, 4), nullptr, 16));
          p += 4;
          if (cp < 0x80) {
            r += static_cast<char>(cp);
          } else if (cp < 0x800) {
            r += static_cast<char>(0xC0 | (cp >> 6));
            r += static_cast<char>(0x80 | (cp & 0x3F));
          } else {
            r += static_cast<char>(0xE0 | (cp >> 12));
            r += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
            r += static_cast<char>(0x80 | (cp & 0x3F));
          }
          break;
        }
        default: r += e;
      }
    }
    if (p >= t.size()) throw std::runtime_error("judge_io: незакрытая строка");
    ++p;  // закрывающая кавычка
    return r;
  }
  static Value parse_value(const std::string& t, std::size_t& p) {
    skip_ws(t, p);
    if (p >= t.size()) throw std::runtime_error("judge_io: пустой JSON");
    char c = t[p];
    if (c == '{') {
      Value v = object();
      ++p;
      skip_ws(t, p);
      if (p < t.size() && t[p] == '}') {
        ++p;
        return v;
      }
      for (;;) {
        skip_ws(t, p);
        if (p >= t.size() || t[p] != '"') throw std::runtime_error("judge_io: ожидался ключ");
        std::string key = parse_string(t, p);
        skip_ws(t, p);
        expect(t, p, ":");
        v.obj_.emplace_back(std::move(key), parse_value(t, p));
        skip_ws(t, p);
        if (p < t.size() && t[p] == ',') {
          ++p;
          continue;
        }
        expect(t, p, "}");
        return v;
      }
    }
    if (c == '[') {
      Value v = array();
      ++p;
      skip_ws(t, p);
      if (p < t.size() && t[p] == ']') {
        ++p;
        return v;
      }
      for (;;) {
        v.arr_.push_back(parse_value(t, p));
        skip_ws(t, p);
        if (p < t.size() && t[p] == ',') {
          ++p;
          continue;
        }
        expect(t, p, "]");
        return v;
      }
    }
    if (c == '"') return Value(parse_string(t, p));
    if (c == 't') {
      expect(t, p, "true");
      return Value(true);
    }
    if (c == 'f') {
      expect(t, p, "false");
      return Value(false);
    }
    if (c == 'n') {
      expect(t, p, "null");
      return Value();
    }
    // число
    std::size_t start = p;
    bool is_float = false;
    if (t[p] == '-' || t[p] == '+') ++p;
    while (p < t.size()) {
      char d = t[p];
      if (d >= '0' && d <= '9') {
        ++p;
      } else if (d == '.' || d == 'e' || d == 'E' || d == '-' || d == '+') {
        is_float = true;
        ++p;
      } else {
        break;
      }
    }
    std::string num = t.substr(start, p - start);
    if (num.empty() || num == "-") throw std::runtime_error("judge_io: ошибка разбора числа");
    if (!is_float) return Value(std::stoll(num));
    return Value(std::stod(num));
  }

  Type t_ = Type::Null;
  bool b_ = false;
  long long i_ = 0;
  double d_ = 0.0;
  std::string s_;
  std::vector<Value> arr_;
  std::vector<std::pair<std::string, Value>> obj_;
};

// Весь stdin → JSON (это поле `input` теста).
inline Value read_input() {
  std::string text((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
  return Value::parse(text);
}

// Одна строка результата в stdout.
inline void emit(const Value& v) {
  std::string s = v.dump();
  std::fwrite(s.data(), 1, s.size(), stdout);
  std::fputc('\n', stdout);
  std::fflush(stdout);
}

// Детерминированный генератор (xorshift32) — одинаковые данные в стенде и в генераторе тестов.
inline std::uint32_t xorshift32(std::uint32_t& s) {
  s ^= s << 13;
  s ^= s >> 17;
  s ^= s << 5;
  return s;
}
inline float uniform01(std::uint32_t& s) { return static_cast<float>(xorshift32(s) >> 8) * (1.0f / 16777216.0f); }

}  // namespace jio
