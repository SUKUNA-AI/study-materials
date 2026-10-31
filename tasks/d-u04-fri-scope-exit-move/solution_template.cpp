#include "solution.h"

class ScopeExit {
  std::function<void()> fn_;
  bool active_ = true;
public:
  explicit ScopeExit(std::function<void()> fn) : fn_(std::move(fn)) {}
  ScopeExit(const ScopeExit&) = delete;
  ScopeExit& operator=(const ScopeExit&) = delete;
  ScopeExit& operator=(ScopeExit&&) = delete;
  ScopeExit(ScopeExit&& other) noexcept : fn_(other.fn_), active_(other.active_) {}
  void dismiss() noexcept { active_ = false; }
  ~ScopeExit() noexcept { if(active_ && fn_) fn_(); }
};
