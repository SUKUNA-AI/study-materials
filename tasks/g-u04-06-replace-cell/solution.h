#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
// Готовый диагностический тип; изменять его не нужно.
struct Cell {
  inline static int live=0, made=0, destroyed=0;
  int value;
  explicit Cell(int v):value(v) { ++live; ++made; }
  ~Cell() { --live; ++destroyed; }
  Cell(const Cell&)=delete;
  Cell& operator=(const Cell&)=delete;
};
void replace_cell(std::unique_ptr<Cell>& owner, int value);
