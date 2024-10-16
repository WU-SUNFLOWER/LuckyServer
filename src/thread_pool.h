#ifndef LUCKYSERVER_THREADPOOL_H_
#define LUCKYSERVER_THREADPOOL_H_

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mutex_;
    std::condition_variable condition_;
    bool stop_;

public:
    explicit ThreadPool(int total = std::thread::hardware_concurrency());

    ~ThreadPool();

    template <class F, class... Args>
    auto AddTask(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::AddTask(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        // don't allow enqueueing after stopping the pool
        if (stop_)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks_.emplace([task]()
                       { (*task)(); });
    }
    condition_.notify_one();
    return res;
}

#endif // LUCKYSERVER_THREADPOOL_H_