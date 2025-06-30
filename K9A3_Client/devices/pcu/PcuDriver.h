#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./PcuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class PcuDriver : public Singleton<PcuDriver>, public DeviceDriver
{
public:
    PcuState* state;

    PcuDriver() : DeviceDriver(DeviceTable::PCU), state(new PcuState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {

    }

    void state_info_ref() override;
};