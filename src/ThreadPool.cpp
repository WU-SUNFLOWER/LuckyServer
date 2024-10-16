#include "ThreadPool.h"
#include "util.h"

ThreadPool::ThreadPool(int total) 
    : stop(false)
{
    for (int i = 0; i < total; ++i) {
        threads.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                // critical section start
                {
                    std::unique_lock<std::mutex> lock(tasks_mutex);
                    condition.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                // critical section end
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        stop = true;
    }
    condition.notify_all();
    // before release ThreadPool itself,
    // let's release all the threads at first!
    for (std::thread& thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

/*
void ThreadPool::addTask(std::function<void()> task) {
    // critical section start
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        if (stop) {
            printErrorAndExit("ThreadPool already stop, can't add task any more");
        }
        tasks.emplace(task);
    }
    // critical section end
    condition.notify_one();
}
*/