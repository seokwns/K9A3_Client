#pragma once

#include <cstdint>

#include "../../libraries/device/DeviceState.h"
#include "./AttlaRawData.h"

/**
 * @brief ATTLA �������� ����ü
 */
#pragma pack(push, 1)
struct AttlaState : public DeviceState
{
    /**
     * @brief ���� ������ġ ��ġ
     */
    float operatingPosition = 0.0f;

    /**
     * @brief RAWDATA
     */
    float motor_position = 0.0f;
    float motor_speed = 0.0f;
    float motor_i = 0.0f;
    float motor_temp = 0.0f;
    float motor_temp_igbt = 0.0f;

    /*
    void update(uint8_t* data) override
    {
        if (data == nullptr)
        {
            std::cerr << std::endl << "[AttlaState] Error: Data pointer is null." << std::endl;
            return;
        }

        AttlaState temp = *reinterpret_cast<AttlaState*>(data);
        *this = temp;
    }

    void print() override
    {
        // ��ġ ���� �� �����׸� ���
        DeviceState::print();

        // raw data ���
        std::cout << "Motor Position          : " << motor_position << std::endl;
        std::cout << "Motor Speed             : " << motor_speed << std::endl;
        std::cout << "Motor I                 : " << motor_i << std::endl;
        std::cout << "Motor Temperature       : " << motor_temp << std::endl;
        std::cout << "IGBT Temperature        : " << motor_temp_igbt << std::endl;
    }
    */
};
#pragma pack(pop)