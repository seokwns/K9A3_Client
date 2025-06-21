#include "Logger.h"

void Logger::log(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printLog(false, fmt, args);
    va_end(args);
}

void Logger::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printLog(true, fmt, args);
    va_end(args);
}

void Logger::logMessage(const ProtocolDataUnit &pdu)
{
    std::ostringstream oss;

    oss << "Header: 0x"
        << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pdu.header.sourceId)
        << std::setw(2) << std::setfill('0') << static_cast<int>(pdu.header.destId)
        << std::setw(2) << std::setfill('0') << static_cast<int>(pdu.header.code)
        << std::setw(2) << std::setfill('0') << static_cast<int>(pdu.header.ack)
        << std::dec
        << " | data (" << static_cast<int>(pdu.header.dataLength) << "):";
    if (pdu.header.dataLength > 0 && pdu.data != nullptr)
    {
        oss << " ";
        for (int i = 0; i < pdu.header.dataLength; ++i)
        {
            oss << "0x"
                << std::uppercase << std::hex
                << std::setw(2) << std::setfill('0')
                << static_cast<int>(pdu.data[i]);
            if (i != pdu.header.dataLength - 1)
                oss << " ";
        }
    }

    Logger::log("%s", oss.str().c_str());
}

void Logger::printLog(bool isError, const char *fmt, va_list args)
{
    char buf[BUF_SIZE];
    vsnprintf(buf, BUF_SIZE, fmt, args);

    uint32_t time = Timer::getInstance().getCurrentTimestamp();
    std::ostringstream oss;
    oss << "[" << std::setw(10) << std::setfill('_') << time / 1000 << "." << std::setw(3) << std::setfill('0') << time % 1000 << "] " << buf;
    std::string logline = oss.str();

    if (isError)
    {
        std::cout << "\033[31m" << logline << "\033[0m" << std::endl;
    }
    else
    {
        std::cout << logline << std::endl;
    }
}