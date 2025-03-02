#include <iostream>
#include <memory>

struct Widget {
private:
  struct Token {};
  int _x;
  int _y;

  Widget(int x, int y) : _x(x), _y(y) {}
        friend class std::shared_ptr<Widget>;
public:
  Widget(int x, int y, Token _tk) : _x(x), _y(y) {} // 公开，但是外部无法调用
  int x() const { return _x; }
  static std::shared_ptr<Widget> make(int x, int y) {
      return std::shared_ptr<Widget>(new Widget(x, y));
  }
  static std::shared_ptr<Widget> makeWithTk(int x, int y) {
      return std::make_shared<Widget>(x, y, Token());
  }
};

int main() {
  auto w = Widget::make(1, 2);
  // auto w = std::make_shared<Widget>(1, 2);
  std::cout << w->x() << std::endl;
  return 0;
}
