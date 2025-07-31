#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./TpcuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class TpcuDriver : public Singleton<TpcuDriver>, public DeviceDriver
{
public:
    TpcuState* state;

    TpcuDriver() : DeviceDriver(DeviceTable::TPCU), state(new TpcuState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {
        DeviceDriver::init();
    }

    void state_info_ref() override;
};