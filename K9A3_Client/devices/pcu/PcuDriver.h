#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "../../libraries/constants/DeviceConstants.h"

#include "./PcuDto.h"
#include "./PcuState.h"

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
        DeviceDriver::init();

        addMessageHandler(0x20, &PcuDriver::TCC_PCU_RESET_UPD);
        addMessageHandler(0x21, &PcuDriver::TCC_PCU_ENCODER_RESET_REQ);
        addMessageHandler(0x22, &PcuDriver::TCC_PCU_BRAKE_CTRL_UPD);
        addMessageHandler(0x32, &PcuDriver::TCC_PCU_VALIDATION_CMD);
        addMessageHandler(0x40, &PcuDriver::TCC_PCU_SINGLE_DRIVE_CMD);
        addMessageHandler(0x41, &PcuDriver::TCC_PCU_SEQUENCE_DRIVE_CMD);
        addMessageHandler(0x42, &PcuDriver::TCC_PCU_POSITION_DRIVE_CMD);
    }

    void PCU_TCC_ENCODER_RESET_RESULT_REF(PCU_TCC_ENCODER_RESET_RESULT_REF_DTO dto);
    void PCU_TCC_VALIDATION_RESULT_UPD(PCU_TCC_VALIDATION_RESULT_UPD_DTO dto);
    void PCU_TCC_INSERT_RESULT_UPD(PCU_TCC_INSERT_RESULT_UPD_DTO dto);
    void PCU_TCC_SINGLE_DRIVE_UPD(PCU_TCC_SINGLE_DRIVE_UPD_DTO dto);
    void PCU_TCC_SEQUENCE_DRIVE_UPD(PCU_TCC_SEQUENCE_DRIVE_UPD_DTO dto);
    void PCU_TCC_POSITION_DRIVE_UPD(PCU_TCC_POSITION_DRIVE_UPD_DTO dto);

    void TCC_PCU_RESET_UPD(const Message& msg);
    void TCC_PCU_ENCODER_RESET_REQ(const Message& msg);
    void TCC_PCU_BRAKE_CTRL_UPD(const Message& msg);
    void TCC_PCU_VALIDATION_CMD(const Message& msg);
    void TCC_PCU_SINGLE_DRIVE_CMD(const Message& msg);
    void TCC_PCU_SEQUENCE_DRIVE_CMD(const Message& msg);
    void TCC_PCU_POSITION_DRIVE_CMD(const Message& msg);

    void state_info_ref() override;
};