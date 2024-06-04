#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    explicit ThreadPool(
        std::size_t numThreads = std::thread::hardware_concurrency())
        : stop(false) {
        for (std::size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        cv.wait(lock,
                                [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for (auto &worker : workers) {
            worker.join();
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            if (stop) {
                throw std::runtime_error("Enqueue on stopped ThreadPool");
            }
            tasks.emplace(std::move(task));
        }
        cv.notify_one();
    }

  private:
    bool stop;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
};