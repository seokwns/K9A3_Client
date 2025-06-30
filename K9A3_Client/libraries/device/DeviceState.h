#pragma once

#include <iostream>

/**
 * @brief 연동 장치들이 주기적으로 전송하는 상태 데이터의 공통 항목
 */
#pragma pack(push, 1)
struct DeviceState
{
    /**
     * @brief CBIT 전송 Flag
     */
    uint16_t cbitFlag = 0x00;

    /**
     * @brief CBIT 전송 주기
     */
    uint16_t cbitPeriod = 0x00;

    /**
     * @brief IBIT 수행 상태
     */
    uint16_t ibitState = 0x00;

    /**
     * @brief 버전정보
     */
    uint16_t majorVersion = 0x00;
    uint16_t minorVersion = 0x00;

    /**
     * @brief 장치구동상태
     */
    uint16_t deviceOperatingState = 0x00;

    /**
     * @brief 바이트 데이터를 업데이트 합니다.
     *
     * @param data 바이트 데이터 포인터
     */
    /*virtual void update(uint8_t* data) {}

    virtual void print()
    {
        std::cout << "DeviceDriver State Information:\n";
        std::cout << "cbitFlag                : 0x" << std::hex << std::uppercase << cbitFlag << std::dec << std::endl;
        std::cout << "cbitPeriod              : 0x" << std::hex << std::uppercase << cbitPeriod << std::dec << std::endl;
        std::cout << "ibitState               : 0x" << std::hex << std::uppercase << ibitState << std::dec << std::endl;
        std::cout << "MajorVersion            : 0x" << std::hex << std::uppercase << majorVersion << std::dec << std::endl;
        std::cout << "MinorVersion            : 0x" << std::hex << std::uppercase << minorVersion << std::dec << std::endl;
        std::cout << "Device Operating State  : 0x" << std::hex << std::uppercase << deviceOperatingState << std::dec << std::endl;
    }*/
};
#pragma pack(pop)