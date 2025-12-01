#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>
std::vector<std::pair<std::string, std::function<void()>>> __tests;

#define LOCATION __FILE__ << ":" << __LINE__ << " "

#define ASSERT(expr, msg) \
  do\
  {\
    if (!(expr)) {\
      std::cerr << LOCATION << "Assertion failed: " << msg << std::endl;\
      std::exit(1);\
    }\
  } while(0)\

#define ASSERT_TRUE(cond) ASSERT(cond, "Expected" #cond " = true, but got false")
#define ASSERT_FALSE(cond) ASSERT(!(cond), "Expected" #cond " = false, but got true")
#define ASSERT_EQ(expected, actual) ASSERT(expected == actual, "Expected" #actual " = " #expected ", but got " #actual)
#define ASSERT_NE(expected, actual) ASSERT(expected != actual, "Expected" #actual " != " #expected ", but got " #actual)

#define TEST_MAIN()                                                            \
  int main() {                                                                 \
    for (auto &test : __tests) {                                               \
      std::cout << "Running " << test.first << "..." << std::endl;             \
      test.second();                                                           \
      std::cout << "Passed " << test.first << std::endl;                       \
    }                                                                          \
    return 0;                                                                  \
  }

class RegistTestCase {
public:
  RegistTestCase(const std::string &name, const std::function<void()> &test) {
    __tests.push_back({name, test});
  }
};

#define TEST(name)                                                             \
  void name();                                                                 \
  RegistTestCase name##_test(#name, name);                                     \
  void name()
