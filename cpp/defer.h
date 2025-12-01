#pragma once

// 辅助宏
#define _DEFER_CONCAT_IMPL(x, y) x##y
#define _DEFER_CONCAT(x, y) _DEFER_CONCAT_IMPL(x, y)

// 注意：这里使用的是直接初始化 Defer var(...)，依靠 C++17 后的 Copy Elision
#define defer(code) ScopeGuard _DEFER_CONCAT(_defer_, __LINE__)([&]() { code; })

template <typename F> class [[nodiscard]] ScopeGuard {
  // no copy and move
public:
  ScopeGuard(F &&f) : _f(f) {}

  ScopeGuard(const ScopeGuard &) = delete;
  ScopeGuard &operator=(const ScopeGuard &) = delete;
  ScopeGuard(ScopeGuard &&) = delete;
  ScopeGuard &operator=(ScopeGuard &&) = delete;

  ~ScopeGuard() { _f(); }

private:
  F _f;
};
