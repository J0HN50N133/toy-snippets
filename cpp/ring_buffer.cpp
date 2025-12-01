// ring buffer

#include "test.hpp"
#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t capacity) : _buffer(capacity), _capacity(capacity), _size(0), _head(0), _tail(0) {}

    void push(const T &value) {
        std::unique_lock<std::mutex> lock(_mutex);
        _full.wait(lock, [this] { return _size < _capacity; });
        _buffer[_tail] = value;
        _tail = (_tail + 1) % _capacity;
        ++_size;
        _empty.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        _empty.wait(lock, [this] { return _size > 0; });
        T value = std::move(_buffer[_head]);
        _head = (_head + 1) % _capacity;
        --_size;
        _full.notify_one();
        return value;
    }

private:
    size_t _capacity;
    size_t _size;
    size_t _head;
    size_t _tail;
    std::vector<T> _buffer;
    std::mutex _mutex;
    std::condition_variable _full;
    std::condition_variable _empty;
};

TEST(test1) {
    RingBuffer<int> buffer(10);
    buffer.push(1);
    ASSERT_EQ(1, buffer.pop());
}

TEST(test2) {
    RingBuffer<int> buffer(10);
    buffer.push(1);
    buffer.push(2);
    ASSERT_EQ(1, buffer.pop());
    ASSERT_EQ(2, buffer.pop());
}

TEST_MAIN()