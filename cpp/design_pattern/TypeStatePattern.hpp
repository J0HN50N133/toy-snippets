#pragma once

#include <string>

/// 1. 【状态·类型】
class State{
private:
        State() = default; // 私有构造函数，禁止外部构造, 只能通过子类构造
        friend class Off;
        friend class On;
};
class Off: public State{
    // 可以定义【Off】的成员函数，独有字段
};
class On: public State{
    // 可以定义【On】的成员函数，独有字段
};

/// 2.【数据·状态】
template <typename State>
class Light {
protected:
    std::string _id; // 公共字段
public:
    Light(std::string id): _id(id) {}
    [[nodiscard]] std::string id() const { return _id; } // 状态共有方法
};

/// 3.【状态·类型】: 状态独有方法
template <>
class Light<Off>: public Light<State>{
public:
    Light(std::string id):Light<State>(std::move(id)) {}
    [[nodiscard]] Light<On> turn_on(this Light<Off>&&);
};
template <>
class Light<On>: public Light<State>{
public:
    Light(std::string id):Light<State>(std::move(id)) {}
    [[nodiscard]] Light<Off> turn_off(this Light<On>&&);
};