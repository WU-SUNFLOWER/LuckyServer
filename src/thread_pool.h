#ifndef LUCKYSERVER_THREADPOOL_H_
#define LUCKYSERVER_THREADPOOL_H_

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool
{
private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable condition_;
    bool stop_;

public:
    ThreadPool(int total = std::thread::hardware_concurrency());

    ~ThreadPool();

    void AddTask(std::function<void()>);
};

#endif // LUCKYSERVER_THREADPOOL_H_