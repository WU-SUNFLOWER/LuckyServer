#include "thread_pool.h"
#include "util.h"

ThreadPool::ThreadPool(int total)
    : stop_(false)
{
    for (int i = 0; i < total; ++i)
    {
        threads_.emplace_back(std::thread([this]()
                                         {
            while (true) {
                std::function<void()> task;
                // critical section start
                {
                    std::unique_lock<std::mutex> lock(tasks_mutex_);
                    condition_.wait(lock, [this]() {
                        return stop_ || !tasks_.empty();
                    });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = tasks_.front();
                    tasks_.pop();
                }
                // critical section end
                task();
            } }));
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    // before release ThreadPool itself,
    // let's release all the threads_ at first!
    for (std::thread &thread : threads_)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void ThreadPool::AddTask(std::function<void()> task)
{
    // critical section start
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        if (stop_)
        {
            util::PrintErrorAndExit("ThreadPool already stop, can't add task any more");
        }
        tasks_.emplace(task);
    }
    // critical section end
    condition_.notify_one();
}
