#include <memory>

// https://en.cppreference.com/w/cpp/language/pimpl
/* .h section */
// 用户依赖的是Impl的声明指针，而不是具体的实现
// 因此如果库修改了Impl的实现，用户不需要重新编译，ABI会保持一致
// 保持ABI稳定的原理：
// - 用户只能依赖Widget, 且只知道Widget的声明和布局，不知道Widget的任何实现
// - 更改Widget的实现，不会影响Widget的布局，因此不会影响用户和库的交互
// - 和private_token.cpp做对比
struct Widget {
private:
  struct Impl;
  std::unique_ptr<Impl> pImpl;
  Widget(std::unique_ptr<Impl> pImpl);

public:
  static std::shared_ptr<Widget> make(int a);
};

struct Widget::Impl {
  Impl(int a) : a(a) {}
  int getA() { return a; }

private:
  int a;
};

/* .cpp section */
std::shared_ptr<Widget> Widget::make(int a) {
    return std::make_shared<Widget>(std::move(std::make_unique<Impl>(a)));
}

Widget::Widget(std::unique_ptr<Impl> pImpl) : pImpl(std::move(pImpl)) {}

int main() {
  auto impl = Widget::make(42);
  return 0;
}
