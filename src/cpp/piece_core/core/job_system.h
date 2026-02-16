#ifndef PIECE_CORE_JOB_SYSTEM_H_
#define PIECE_CORE_JOB_SYSTEM_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Piece::Core
{

class JobSystem
{
  public:
    explicit JobSystem(size_t num_threads = std::thread::hardware_concurrency());
    ~JobSystem();

    // Enqueues a task to be executed by a worker thread.
    // Returns a future that will hold the result of the task.
    template <class F, class... Args> auto Enqueue(F &&f, Args &&...args) -> std::future<std::result_of_t<F(Args...)>>;

    // Waits for all currently enqueued tasks to complete.
    void WaitUntilAllDone();

  private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
    std::atomic<int> busy_tasks_; // Counter for active tasks

    void WorkerLoop();
};

// Implementation of Enqueue template method
template <class F, class... Args>
auto JobSystem::Enqueue(F &&f, Args &&...args) -> std::future<std::result_of_t<F(Args...)>>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task =
        std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        if (stop_)
        {
            throw std::runtime_error("Enqueue on stopped JobSystem");
        }
        tasks_.emplace([task]() { (*task)(); });
        busy_tasks_.fetch_add(1);
    }
    condition_.notify_one();
    return res;
}

} // namespace Piece::Core

#endif // PIECE_CORE_JOB_SYSTEM_H_
