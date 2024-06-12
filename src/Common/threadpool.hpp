#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  private:
    bool m_stop;
    mutable std::mutex m_mtx;
    std::condition_variable m_cv;
    std::vector<std::jthread> m_threads;
    std::queue<std::function<void()>> m_tasks;

  public:
    explicit ThreadPool(
        std::size_t numThreads = std::thread::hardware_concurrency())
        : m_stop(false) {
        auto task = [this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(m_mtx);
                    m_cv.wait(lock,
                              [this] { return m_stop || !m_tasks.empty(); });
                    if (m_stop && m_tasks.empty()) {
                        break;
                    }
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                task();
            }
        };
        for (std::size_t i = 0; i < numThreads; ++i) {
            m_threads.emplace_back(std::jthread(task));
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_stop = true;
        }
        m_cv.notify_all();
        for (auto &thread : m_threads) {
            thread.join();
        }
    }

    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type>
    enqueue(F &&f, Args &&...args) {
        using returnType = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            if (m_stop) {
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            }
            m_tasks.emplace([task]() { (*task)(); });
        }
        m_cv.notify_one();
        return res;
    }
};