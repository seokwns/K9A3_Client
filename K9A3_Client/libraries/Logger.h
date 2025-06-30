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
#include "./singleton.h"
#include "./messages.h"

class Logger : public Singleton<Logger>
{
public:
    void log(const char *fmt, ...);

    void logMessage(const ProtocolDataUnit &pdu);

    void error(const char *fmt, ...);

private:
    static constexpr size_t BUF_SIZE = 1024;

    void printLog(bool isError, const char *fmt, va_list args);
};
