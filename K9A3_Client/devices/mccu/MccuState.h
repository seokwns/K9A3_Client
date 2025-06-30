#pragma once

#include <cstdint>

#include "../../libraries/device/DeviceState.h"

/**
 * @brief ATTLA �������� ����ü
 */
#pragma pack(push, 1)
struct MccuState : public DeviceState
{
    /**
     * @brief ���� ������ġ ��ġ
     */
    float operatingPosition = 0.0f;
};
#pragma pack(pop)