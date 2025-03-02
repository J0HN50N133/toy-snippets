#include <iostream>

struct Base {
  void foo(this auto &&self) {
    std::cout << "Base::foo() called with derived type: " << typeid(self).name()
              << std::endl;
  }
};

struct Derived : Base {
  void bar() {
    foo(); // 调用基类的 foo，自动推导出 Derived 类型, 可以替代CRTP
  }
};

int main() {
  Derived d;
  d.bar(); // 输出: Base::foo() called with derived type: Derived
  return 0;
}
