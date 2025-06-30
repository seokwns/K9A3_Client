#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <ctime>
#include <thread>

#include "./singleton.h"

class Timer : public Singleton<Timer>
{
private:
    std::chrono::steady_clock::time_point start_time;

public:
    Timer()
    {
        start_time = std::chrono::steady_clock::now();
    }

    uint32_t getCurrentTimestamp() const
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
        return static_cast<uint32_t>(elapsed.count());
    }

    std::tm *getUTC() const
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        return std::gmtime(&currentTime);
    }

    static void wait(uint64_t milli)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milli));
    }
};