#include "ThreadPool.h"

#include <iostream>

ThreadPool::ThreadPool(size_t num_threads, MessageHandler handler)
    : stop(false), handlePacketData(handler)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutexForQueue);
        stop = true;
    }
    cvForQueue.notify_all();
    for (auto &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::enqueue(const Message &msg)
{
    {
        std::lock_guard<std::mutex> lock(mutexForQueue);
        messages.push(msg);
    }

    cvForQueue.notify_one();
}

void ThreadPool::worker_thread()
{
    while (true)
    {
        Message message;

        {
            std::unique_lock<std::mutex> lock(mutexForQueue);
            cvForQueue.wait(lock, [this]
                            { return stop || !messages.empty(); });

            if (stop && messages.empty())
            {
                return;
            }

            if (!messages.empty())
            {
                message = messages.top();
                messages.pop();
            }
        }

        handlePacketData(message);
    }
}