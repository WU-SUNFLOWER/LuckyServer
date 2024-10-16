#ifndef HEAD_THREADPOOL
#define HEAD_THREADPOOL

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mutex;
    std::condition_variable condition;
    bool stop;
public:
    ThreadPool(int total = std::thread::hardware_concurrency());
    
    ~ThreadPool();

    void addTask(std::function<void()>);
};

#endif