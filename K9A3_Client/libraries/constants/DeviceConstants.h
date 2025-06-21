#pragma once

#include <cstdint>

struct DeviceRecord
{
    uint8_t deviceId;
    char ip[16];
    uint16_t port;
};

namespace DeviceTable
{
    // TODO: ip mapping 결정 완료 후 테이블에 추가하기
    constexpr DeviceRecord TCC = {0x10, "192.168.0.100", 9000};
    constexpr DeviceRecord ATTLA = {0x22, "192.168.0.100", 9003};
}
