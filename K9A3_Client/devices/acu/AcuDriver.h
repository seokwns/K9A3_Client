#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./AcuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class AcuDriver : public Singleton<AcuDriver>, public DeviceDriver
{
public:
    AcuState* state;

    AcuDriver() : DeviceDriver(DeviceTable::ACU), state(new AcuState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {

    }

    void state_info_ref() override;
};