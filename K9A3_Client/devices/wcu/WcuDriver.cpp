#include "./WcuDriver.h"


void WcuDriver::state_info_ref()
{
    sendMessage(MessageCode::STATE_INFO, AckCode::NO_ACK, static_cast<uint8_t>(sizeof(*state)), state);
}

void WcuDriver::WCU_TCC_VALIDATION_RESULT_UPD(WCU_TCC_VALIDATION_RESULT_UPD_DTO dto)
{
    sendMessage(0x32, AckCode::NO_ACK, sizeof(dto), &dto);
}

void WcuDriver::WCU_TCC_RECOIL_TIME_REF(WCU_TCC_RECOIL_TIME_REF_DTO dto)
{
    sendMessage(0x33, AckCode::NO_ACK, sizeof(dto), &dto);
}

void WcuDriver::WCU_TCC_GUN_BREECH_CTRL_UPD(WCU_TCC_GUN_BREECH_CTRL_UPD_DTO dto)
{
    sendMessage(0x40, AckCode::NO_ACK, sizeof(dto), &dto);
}

void WcuDriver::WCU_TCC_FIRE_RESULT_REF(WCU_TCC_FIRE_RESULT_REF_DTO dto)
{
    sendMessage(0x41, AckCode::NO_ACK, sizeof(dto), &dto);
}

void WcuDriver::TCC_WCU_RESET_UPD(const Message& msg)
{
}

void WcuDriver::TCC_WCU_VALIDATION_CMD(const Message& msg)
{
    TCC_WCU_VALIDATION_CMD_DTO dto = msg.pdu.getData<TCC_WCU_VALIDATION_CMD_DTO>();
    Timer::wait(400);
    WCU_TCC_VALIDATION_RESULT_UPD({ dto.command, dto.axis, 0x01 });
}

void WcuDriver::TCC_WCU_RECOIL_TIME_REQ(const Message& msg)
{
    WCU_TCC_RECOIL_TIME_REF({ 1400 });
}

void WcuDriver::TCC_WCU_GUN_BREECH_CTRL_CMD(const Message& msg)
{
    TCC_WCU_GUN_BREECH_CTRL_CMD_DTO dto = msg.pdu.getData<TCC_WCU_GUN_BREECH_CTRL_CMD_DTO>();
    Timer::wait(400);
    WCU_TCC_GUN_BREECH_CTRL_UPD({ dto.action, 0x01 });
}
