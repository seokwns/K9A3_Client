#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./MccuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class MccuDriver : public Singleton<MccuDriver>, public DeviceDriver
{
public:
    MccuState* state;

    MccuDriver() : DeviceDriver(DeviceTable::MCCU), state(new MccuState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {

    }

    void state_info_ref() override;
};