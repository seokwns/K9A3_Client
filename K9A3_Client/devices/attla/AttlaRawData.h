#pragma once

#include "../../libraries/device/DeviceRawData.h"

struct AttlaRawData : public DeviceRawData
{
    float motor_position = 0.0f;
    float motor_speed = 0.0f;
    float motor_i = 0.0f;
    float motor_temp = 0.0f;
    float motor_temp_igbt = 0.0f;

    void print() override
    {
        std::cout << "Motor Position          : " << motor_position << std::endl;
        std::cout << "Motor Speed             : " << motor_speed << std::endl;
        std::cout << "Motor I                 : " << motor_i << std::endl;
        std::cout << "Motor Temperature       : " << motor_temp << std::endl;
        std::cout << "IGBT Temperature        : " << motor_temp_igbt << std::endl;
    }
};