#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <cstdarg>
#include <vector>
#include <cstdio>
#include <iomanip>
#include <sstream>

#include "./Timer.h"

namespace
{
    constexpr const char* COLOR_RED = "\033[31m";
    constexpr const char* COLOR_RESET = "\033[0m";
    constexpr const char* COLOR_YELLOW = "\033[33m";
    constexpr const char* COLOR_GREEN = "\033[32m";
    constexpr const char* COLOR_BLUE = "\033[34m";
    constexpr const char* COLOR_GRAY = "\033[90m";
    constexpr const char* COLOR_DEFAULT = "\033[39m";
}

enum LogLevel
{
    LOG_ALL,
    LOG_INFO
};

class Logger : public Singleton<Logger>
{
public:
    static void log(const char* fmt, ...);

    static void debug(const char* fmt, ...);

    static void info(const char* fmt, ...);

    static void warning(const char* fmt, ...);

    static void error(const char* fmt, ...);

private:
    static constexpr size_t BUF_SIZE = 1024;

    static void vprintLog(const char* color, const char* fmt, va_list args);
};
