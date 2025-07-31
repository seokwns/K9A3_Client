#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "../../libraries/constants/DeviceConstants.h"

#include "./MccuDto.h"
#include "./MccuState.h"

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
        DeviceDriver::init();

        addMessageHandler(0x20, &MccuDriver::TCC_MCCU_RESET_UPD);
        addMessageHandler(0x21, &MccuDriver::TCC_MCCU_ENCODER_RESET_REQ);
        addMessageHandler(0x22, &MccuDriver::TCC_MCCU_BRAKE_CTRL_UPD);
        addMessageHandler(0x32, &MccuDriver::TCC_MCCU_VALIDATION_CMD);
        addMessageHandler(0x40, &MccuDriver::TCC_MCCU_SINGLE_DRIVE_CMD);
        addMessageHandler(0x41, &MccuDriver::TCC_MCCU_SEQUENCE_DRIVE_CMD);
        addMessageHandler(0x42, &MccuDriver::TCC_MCCU_POSITION_DRIVE_CMD);
    }

    void state_info_ref() override;
    
    void MCCU_TCC_ENCODER_RESET_RESULT_REF(MCCU_TCC_ENCODER_RESET_RESULT_REF_DTO dto);
    void MCCU_TCC_VALIDATION_RESULT_UPD(MCCU_TCC_VALIDATION_RESULT_UPD_DTO dto);
    void MCCU_TCC_SINGLE_DRIVE_UPD(MCCU_TCC_SINGLE_DRIVE_UPD_DTO dto);
    void MCCU_TCC_SEQUENCE_DRIVE_UPD(MCCU_TCC_SEQUENCE_DRIVE_UPD_DTO dto);
    void MCCU_TCC_POSITION_DRIVE_UPD(MCCU_TCC_POSITION_DRIVE_UPD_DTO dto);

    void TCC_MCCU_RESET_UPD(const Message& msg);
    void TCC_MCCU_ENCODER_RESET_REQ(const Message& msg);
    void TCC_MCCU_BRAKE_CTRL_UPD(const Message& msg);
    void TCC_MCCU_VALIDATION_CMD(const Message& msg);
    void TCC_MCCU_SINGLE_DRIVE_CMD(const Message& msg);
    void TCC_MCCU_SEQUENCE_DRIVE_CMD(const Message& msg);
    void TCC_MCCU_POSITION_DRIVE_CMD(const Message& msg);
};