#pragma once

#include <cstdint>

struct DeviceRecord
{
    char name[6];
    uint8_t deviceId;
    char ip[16];
    uint16_t port;
    uint16_t cbitPeriod; // N Hz
};

namespace DeviceTable
{
    // TODO: ip mapping 결정 완료 후 테이블에 추가하기
    constexpr DeviceRecord TCC = { "TCC", 0x10, "192.168.100.100", 9000, 10 };
    constexpr DeviceRecord RDU = { "RDU", 0x13, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord GTCU = { "GTCU", 0x20, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord WCU = { "WCU", 0x21, "192.168.100.100", 9003, 10 };
    // 선회잠금장치 확인 필요 (0x22 ? 0x90 ?)
    constexpr DeviceRecord ATTLA = { "ATTLA", 0x22, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord PCU = { "PCU", 0x30, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord MCCU = { "MCCU", 0x31, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord ACU = { "ACU", 0x32, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord TPCU = { "TPCU", 0x40, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord TBU = { "TBU", 0x41, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord FSVU = { "FSVU", 0x60, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord ILP = { "ILP", 0x70, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord HIC = { "HIC", 0x71, "192.168.100.100", 9003, 10 };

    constexpr DeviceRecord MCS = { "MCS", 0xA1, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord OCS = { "OCS", 0xA2, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord TCU = { "TCU", 0xA3, "192.168.100.100", 9003, 10 };
    // 단말기도 CCU, 무장제어기도 CCU 수정 필요

    constexpr DeviceRecord APU = { "APU", 0x80, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord VIC = { "VIC", 0xB1, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord RDC = { "RDC", 0xB2, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord FCAM = { "FCAM", 0xB3, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord RCAM = { "RCAM", 0xB4, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord ACAM = { "ACAM", 0xB5, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord ADC = { "ADC", 0xB6, "192.168.100.100", 9003, 10 };
    constexpr DeviceRecord SCS = { "SCS", 0xB7, "192.168.100.100", 9003, 10 };

    constexpr DeviceRecord DIP = { "DIP", 0xC0, "192.168.100.100", 9003, 10 };
}
