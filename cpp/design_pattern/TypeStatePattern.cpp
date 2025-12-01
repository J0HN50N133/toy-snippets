#include "TypeStatePattern.hpp"

#include <iostream>

/// this will take the ownership of class Light<Off>
Light<On> Light<Off>::turn_on(this Light<Off>&& self) {
  std::cout << "Light "<< self._id << " is on" << std::endl;
  return Light<On>(std::move(self._id)); // transition to on state
}

Light<Off> Light<On>::turn_off(this Light<On>&& self) {
  std::cout << "Light "<< self._id << " is off" << std::endl;
  return Light<Off>(std::move(self._id)); // transition to off state
}

int main() {
  Light<Off> light("1");
  auto on = std::move(light).turn_on();
  auto on2 = std::move(light).turn_on();
  // on.turn_on(); // it's impossible to call turn_on() on off state
  auto off = std::move(on).turn_off();
  return 0;
}