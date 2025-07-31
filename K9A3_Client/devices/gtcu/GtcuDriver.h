#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./GtcuState.h"
#include "../../libraries/constants/DeviceConstants.h"

class GtcuDriver : public Singleton<GtcuDriver>, public DeviceDriver
{
public:
    GtcuState* state;

    GtcuDriver() : DeviceDriver(DeviceTable::GTCU), state(new GtcuState)
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