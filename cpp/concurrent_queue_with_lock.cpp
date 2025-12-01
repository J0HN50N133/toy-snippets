#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "test.hpp"

template <typename T> class Channel {
public:
  Channel(size_t capacity) : _capacity(capacity), _size(0) {}

  void push(const T &value) {
    std::unique_lock<std::mutex> lock(_mutex);
    _full.wait(lock, [this] { return _size < _capacity; });
    _queue.push(value);
    ++_size;
    _empty.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _empty.wait(lock, [this] { return _size > 0; });
    T value = std::move(_queue.front());
    _queue.pop();
    --_size;
    _full.notify_one();
    return value;
  }

private:
  std::queue<T> _queue;
  std::mutex _mutex;
  std::condition_variable _full;
  std::condition_variable _empty;
  size_t _capacity;
  size_t _size;
};

TEST(test1) {
    Channel<int> channel(1);
    channel.push(1);
    ASSERT_EQ(1, channel.pop());
}

TEST(test2) {
    Channel<int> channel(1);
    channel.push(1);
    channel.pop();
    channel.push(2);
    ASSERT_EQ(2, channel.pop());
}

TEST(Multithread) {
    Channel<int> channel(1);
    std::thread t1([&channel] {
        channel.push(1);
        channel.push(2);
    });
    std::thread t2([&channel] {
        ASSERT_EQ(1, channel.pop());
        ASSERT_EQ(2, channel.pop());
    });
    t1.join();
    t2.join();
}

TEST_MAIN();
