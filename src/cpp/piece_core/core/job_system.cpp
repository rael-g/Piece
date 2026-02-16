#include "job_system.h"
#include <piece_core/logging_api.h>

namespace Piece::Core
{

JobSystem::JobSystem(size_t num_threads) : stop_(false), busy_tasks_(0)
{
    PIECE_TRACE("JobSystem constructor called.");
    if (num_threads == 0)
    {
        num_threads = 1; // Ensure at least one thread
        PIECE_WARN("num_threads was 0, defaulting to 1 thread for JobSystem.");
    }
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back([this] { this->WorkerLoop(); });
        PIECE_DEBUG("JobSystem: Worker thread {0} started.", i + 1);
    }
    PIECE_INFO("JobSystem initialized with {0} threads.", num_threads);
}

JobSystem::~JobSystem()
{
    PIECE_TRACE("JobSystem destructor called. Shutting down...");
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (std::thread &worker : workers_)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
    PIECE_INFO("JobSystem shut down. All worker threads joined.");
}

void JobSystem::WorkerLoop()
{
    PIECE_TRACE("JobSystem: WorkerLoop started for thread {0}.", std::this_thread::get_id());
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty())
            {
                PIECE_TRACE("JobSystem: WorkerLoop for thread {0} exiting.", std::this_thread::get_id());
                return;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
            PIECE_DEBUG("JobSystem: Thread {0} picked up a task.", std::this_thread::get_id());
        }
        task();
        busy_tasks_.fetch_sub(1); // Decrement busy count after task completes
        PIECE_DEBUG("JobSystem: Thread {0} completed a task. Remaining busy tasks: {1}", std::this_thread::get_id(),
                    busy_tasks_.load());
    }
}

void JobSystem::WaitUntilAllDone()
{
    PIECE_TRACE("JobSystem::WaitUntilAllDone called. Waiting for {0} busy tasks...", busy_tasks_.load());
    // Wait until busy_tasks_ count is 0
    while (busy_tasks_.load() > 0)
    {
        std::this_thread::yield(); // Yield to allow other threads to run
    }
    PIECE_INFO("JobSystem::WaitUntilAllDone finished. All tasks completed.");
}

} // namespace Piece::Core
