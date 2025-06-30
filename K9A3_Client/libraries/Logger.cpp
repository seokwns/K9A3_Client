#include "Logger.h"

void Logger::log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintLog(COLOR_GRAY, fmt, args);
    va_end(args);
}

void Logger::debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintLog(COLOR_DEFAULT, fmt, args);
    va_end(args);
}

void Logger::info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintLog(COLOR_GREEN, fmt, args);
    va_end(args);
}

void Logger::warning(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintLog(COLOR_YELLOW, fmt, args);
    va_end(args);
}

void Logger::error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintLog(COLOR_RED, fmt, args);
    va_end(args);
}

void Logger::vprintLog(const char* color, const char* fmt, va_list args)
{
    char buf[BUF_SIZE];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);

    if (len < 0 || static_cast<size_t>(len) >= sizeof(buf))
    {
        buf[sizeof(buf) - 4] = '.';
        buf[sizeof(buf) - 3] = '.';
        buf[sizeof(buf) - 2] = '.';
        buf[sizeof(buf) - 1] = '\0';
    }

    uint32_t time = Timer::getInstance().getCurrentTimestamp();

    std::ostringstream ossHeader;

    ossHeader << '['
        << std::setw(10) << std::setfill('_') << time / 1000
        << '.'
        << std::setw(3) << std::setfill('0') << time % 1000
        << "] ";

    std::string header = ossHeader.str();
    std::istringstream iss(buf);
    std::string line;

    while (std::getline(iss, line))
    {
        std::cout << color << header << line << COLOR_RESET << std::endl;
    }
    std::cout.flush();
}