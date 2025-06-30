#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./WcuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class WcuDriver : public Singleton<WcuDriver>, public DeviceDriver
{
public:
    WcuState* state;

    WcuDriver() : DeviceDriver(DeviceTable::WCU), state(new WcuState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {

    }

    void state_info_ref() override;
};