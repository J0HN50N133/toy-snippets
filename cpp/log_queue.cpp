// log queue only keep the last N logs

#include "test.hpp"
#include <vector>
#include <mutex>
#include <condition_variable>

class Log {
private:
    std::string _log;
    uint64_t _timestamp;
    uint64_t _thread_id;
public:
    Log(){}
    Log(std::string log, uint64_t timestamp, uint64_t thread_id) : _log(std::move(log)), _timestamp(timestamp), _thread_id(thread_id) {}
};

class LogQueue {
private:
    std::vector<Log> _logs;
    size_t _capacity;
    size_t _size;
    size_t _head;
    size_t _tail;
    std::mutex _mutex;
public:
    LogQueue(size_t capacity) :_logs(capacity), _capacity(capacity), _head(0), _tail(0), _size(0) {}
    void push(const Log& log) {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_size == _capacity) {
            _logs[_tail % _capacity] = log;
            _head++;
        } else {
            _logs[_tail % _capacity] = log;
            _size++;
        }
        _tail++;
    }
    Log pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        Log log = _logs[_head];
        _head = (_head + 1) % _capacity;
        return log;
    }
};

TEST_MAIN()
