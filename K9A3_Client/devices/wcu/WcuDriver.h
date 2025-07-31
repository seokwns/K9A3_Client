#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "../../libraries/constants/DeviceConstants.h"

#include "./WcuDto.h"
#include "./WcuState.h"

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
        DeviceDriver::init();

        addMessageHandler(0x20, &WcuDriver::TCC_WCU_RESET_UPD);
        addMessageHandler(0x32, &WcuDriver::TCC_WCU_VALIDATION_CMD);
        addMessageHandler(0x33, &WcuDriver::TCC_WCU_RECOIL_TIME_REQ);
        addMessageHandler(0x40, &WcuDriver::TCC_WCU_GUN_BREECH_CTRL_CMD);
    }

    void state_info_ref() override;

    void WCU_TCC_VALIDATION_RESULT_UPD(WCU_TCC_VALIDATION_RESULT_UPD_DTO dto);
    void WCU_TCC_RECOIL_TIME_REF(WCU_TCC_RECOIL_TIME_REF_DTO dto);
    void WCU_TCC_GUN_BREECH_CTRL_UPD(WCU_TCC_GUN_BREECH_CTRL_UPD_DTO dto);
    void WCU_TCC_FIRE_RESULT_REF(WCU_TCC_FIRE_RESULT_REF_DTO dto);

    void TCC_WCU_RESET_UPD(const Message& msg);
    void TCC_WCU_VALIDATION_CMD(const Message& msg);
    void TCC_WCU_RECOIL_TIME_REQ(const Message& msg);
    void TCC_WCU_GUN_BREECH_CTRL_CMD(const Message& msg);
};