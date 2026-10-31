#pragma once
#include <memory>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <array>
struct Node {
  inline static int live=0;
  std::shared_ptr<Node> child;
  std::weak_ptr<Node> parent;
  Node() { ++live; }
  ~Node() { --live; }
  Node(const Node&)=delete;
  Node& operator=(const Node&)=delete;
};
void link_nodes(const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& child);
