#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "../../libraries/constants/DeviceConstants.h"

#include "./AcuDto.h"
#include "./AcuState.h"

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
        DeviceDriver::init();

        addMessageHandler(0x20, &AcuDriver::TCC_ACU_RESET_UPD);
        addMessageHandler(0x21, &AcuDriver::TCC_ACU_ENCODER_RESET_REQ);
        addMessageHandler(0x22, &AcuDriver::TCC_ACU_BRAKE_CTRL_UPD);
        addMessageHandler(0x32, &AcuDriver::TCC_ACU_VALIDATION_CMD);
        addMessageHandler(0x33, &AcuDriver::TCC_ACU_PASS_PROJECTILE_CMD);
        addMessageHandler(0x34, &AcuDriver::TCC_ACU_PASS_CHARGE_CMD);
        addMessageHandler(0x40, &AcuDriver::TCC_ACU_SINGLE_DRIVE_CMD);
        addMessageHandler(0x41, &AcuDriver::TCC_ACU_ALIGN_GUN_BREECH_CMD);
        addMessageHandler(0x42, &AcuDriver::TCC_ACU_LOAD_PROJECTILE_CMD);
        addMessageHandler(0x43, &AcuDriver::TCC_ACU_LOAD_CHARGE_CMD);
        addMessageHandler(0x44, &AcuDriver::TCC_ACU_POSITION_DRIVE_CMD);
        addMessageHandler(0x45, &AcuDriver::TCC_ACU_OFFSET_VALUE_CMD);
    }

    void state_info_ref() override;

    void ACU_TCC_ENCODER_RESET_RESULT_REF(ACU_TCC_ENCODER_RESET_RESULT_REF_DTO dto);
    void ACU_TCC_VALIDATION_RESULT_UPD(ACU_TCC_VALIDATION_RESULT_UPD_DTO dto);
    void ACU_TCC_PASS_PROJECTILE_REF(ACU_TCC_PASS_PROJECTILE_REF_DTO dto);
    void ACU_TCC_PASS_CHARGE_REF(ACU_TCC_PASS_CHARGE_REF_DTO dto);
    void ACU_TCC_SINGLE_DRIVE_UPD(ACU_TCC_SINGLE_DRIVE_UPD_DTO dto);
    void ACU_TCC_ALIGN_GUN_BREECH_UPD(ACU_TCC_ALIGN_GUN_BREECH_UPD_DTO dto);
    void ACU_TCC_LOAD_PROJECTILE_UPD(ACU_TCC_LOAD_PROJECTILE_UPD_DTO dto);
    void ACU_TCC_LOAD_CHARGE_UPD(ACU_TCC_LOAD_CHARGE_UPD_DTO dto);
    void ACU_TCC_POSITION_DRIVE_UPD(ACU_TCC_POSITION_DRIVE_UPD_DTO dto);
    void ACU_TCC_OFFSET_VALUE_REF(ACU_TCC_OFFSET_VALUE_REF_DTO dto);

    void TCC_ACU_RESET_UPD(const Message& msg);
    void TCC_ACU_ENCODER_RESET_REQ(const Message& msg);
    void TCC_ACU_BRAKE_CTRL_UPD(const Message& msg);
    void TCC_ACU_VALIDATION_CMD(const Message& msg);
    void TCC_ACU_PASS_PROJECTILE_CMD(const Message& msg);
    void TCC_ACU_PASS_CHARGE_CMD(const Message& msg);
    void TCC_ACU_SINGLE_DRIVE_CMD(const Message& msg);
    void TCC_ACU_ALIGN_GUN_BREECH_CMD(const Message& msg);
    void TCC_ACU_LOAD_PROJECTILE_CMD(const Message& msg);
    void TCC_ACU_LOAD_CHARGE_CMD(const Message& msg);
    void TCC_ACU_POSITION_DRIVE_CMD(const Message& msg);
    void TCC_ACU_OFFSET_VALUE_CMD(const Message& msg);
};