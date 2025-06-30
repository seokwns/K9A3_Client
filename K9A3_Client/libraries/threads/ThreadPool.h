#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "../messages.h"

class ThreadPool
{
public:
    using MessageHandler = std::function<void(const Message &)>;

    ThreadPool(size_t num_threads, MessageHandler handler);
    ~ThreadPool();

    void enqueue(const Message &msg);

private:
    void worker_thread();

    std::vector<std::thread> workers;
    std::priority_queue<Message> messages;
    std::mutex mutexForQueue;
    std::condition_variable cvForQueue;
    bool stop;
    MessageHandler handlePacketData;
};