#pragma once

#include "../../libraries/singleton.h"
#include "../../libraries/tcc/TurretControl.h"
#include "../../libraries/device/DeviceDriver.h"
#include "./AttlaState.h"
#include "../../libraries/constants/DeviceConstants.h"

class AttlaDriver : public Singleton<AttlaDriver>, public DeviceDriver
{
public:
    AttlaState* state;

    AttlaDriver() : DeviceDriver(DeviceTable::ATTLA), state(new AttlaState)
    {
        state->deviceOperatingState = 0x12;
        state->operatingPosition = 10.0f;
    }

    void init() override
    {
        addMessageHandler(0x20, &AttlaDriver::tcc_attla_reset_upd);
        addMessageHandler(0x21, &AttlaDriver::tcc_attla_encoder_reset_req);
        addMessageHandler(0x31, &AttlaDriver::tcc_attla_drive_cmd_cancel_cmd);
        addMessageHandler(0x32, &AttlaDriver::tcc_attla_validation_cmd);
        addMessageHandler(0x40, &AttlaDriver::tcc_attla_drive_cmd);
    }

    void tcc_attla_reset_upd(const Message& msg);
    void tcc_attla_encoder_reset_req(const Message& msg);
    void tcc_attla_drive_cmd_cancel_cmd(const Message& msg);
    void tcc_attla_validation_cmd(const Message& msg);
    void tcc_attla_drive_cmd(const Message& msg);

    void attla_tcc_encoder_reset_result_ref(unsigned short target, unsigned short result);
    void attla_tcc_drive_cmd_cancel_ref(unsigned short result);
    void attla_tcc_validation_result_upd(unsigned short target, unsigned short result);
    void attla_tcc_drive_upd(unsigned short drive_cmd, unsigned short result);
    void state_info_ref() override;
};