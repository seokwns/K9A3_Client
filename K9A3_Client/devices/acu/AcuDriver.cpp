#include "./AcuDriver.h"


void AcuDriver::state_info_ref()
{
    sendMessage(MessageCode::STATE_INFO, AckCode::NO_ACK, static_cast<uint8_t>(sizeof(*state)), state);
}

void AcuDriver::ACU_TCC_ENCODER_RESET_RESULT_REF(ACU_TCC_ENCODER_RESET_RESULT_REF_DTO dto)
{
    sendMessage(0x21, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_VALIDATION_RESULT_UPD(ACU_TCC_VALIDATION_RESULT_UPD_DTO dto)
{
    sendMessage(0x32, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_PASS_PROJECTILE_REF(ACU_TCC_PASS_PROJECTILE_REF_DTO dto)
{
    sendMessage(0x33, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_PASS_CHARGE_REF(ACU_TCC_PASS_CHARGE_REF_DTO dto)
{
    sendMessage(0x34, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_SINGLE_DRIVE_UPD(ACU_TCC_SINGLE_DRIVE_UPD_DTO dto)
{
    sendMessage(0x40, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_ALIGN_GUN_BREECH_UPD(ACU_TCC_ALIGN_GUN_BREECH_UPD_DTO dto)
{
    sendMessage(0x41, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_LOAD_PROJECTILE_UPD(ACU_TCC_LOAD_PROJECTILE_UPD_DTO dto)
{
    sendMessage(0x42, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_LOAD_CHARGE_UPD(ACU_TCC_LOAD_CHARGE_UPD_DTO dto)
{
    sendMessage(0x43, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_POSITION_DRIVE_UPD(ACU_TCC_POSITION_DRIVE_UPD_DTO dto)
{
    sendMessage(0x44, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::ACU_TCC_OFFSET_VALUE_REF(ACU_TCC_OFFSET_VALUE_REF_DTO dto)
{
    sendMessage(0x45, AckCode::NO_ACK, sizeof(dto), &dto);
}

void AcuDriver::TCC_ACU_RESET_UPD(const Message& msg)
{
}

void AcuDriver::TCC_ACU_ENCODER_RESET_REQ(const Message& msg)
{
    TCC_ACU_ENCODER_RESET_REQ_DTO dto = msg.pdu.getData<TCC_ACU_ENCODER_RESET_REQ_DTO>();
    Timer::wait(400);
    ACU_TCC_ENCODER_RESET_RESULT_REF({ dto.target, 0x01 });
}

void AcuDriver::TCC_ACU_BRAKE_CTRL_UPD(const Message& msg)
{
}

void AcuDriver::TCC_ACU_VALIDATION_CMD(const Message& msg)
{
    TCC_ACU_VALIDATION_CMD_DTO dto = msg.pdu.getData<TCC_ACU_VALIDATION_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_VALIDATION_RESULT_UPD({ dto.command, dto.axis, 0x01 });
}

void AcuDriver::TCC_ACU_PASS_PROJECTILE_CMD(const Message& msg)
{
    Timer::wait(400);
    ACU_TCC_PASS_PROJECTILE_REF({ 0x01 });
}

void AcuDriver::TCC_ACU_PASS_CHARGE_CMD(const Message& msg)
{
    Timer::wait(400);
    ACU_TCC_PASS_CHARGE_REF({ 0x01 });
}

void AcuDriver::TCC_ACU_SINGLE_DRIVE_CMD(const Message& msg)
{
    TCC_ACU_SINGLE_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_ACU_SINGLE_DRIVE_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_SINGLE_DRIVE_UPD({ dto.axis, dto.action, 0x01 });
}

void AcuDriver::TCC_ACU_ALIGN_GUN_BREECH_CMD(const Message& msg)
{
    TCC_ACU_ALIGN_GUN_BREECH_CMD_DTO dto = msg.pdu.getData<TCC_ACU_ALIGN_GUN_BREECH_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_ALIGN_GUN_BREECH_UPD({ dto.seq, 0x01 });
}

void AcuDriver::TCC_ACU_LOAD_PROJECTILE_CMD(const Message& msg)
{
    TCC_ACU_LOAD_PROJECTILE_CMD_DTO dto = msg.pdu.getData<TCC_ACU_LOAD_PROJECTILE_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_LOAD_PROJECTILE_UPD({ dto.seq, 0x01 });
}

void AcuDriver::TCC_ACU_LOAD_CHARGE_CMD(const Message& msg)
{
    TCC_ACU_LOAD_CHARGE_CMD_DTO dto = msg.pdu.getData<TCC_ACU_LOAD_CHARGE_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_LOAD_CHARGE_UPD({ dto.seq, 0x01 });
}

void AcuDriver::TCC_ACU_POSITION_DRIVE_CMD(const Message& msg)
{
    TCC_ACU_POSITION_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_ACU_POSITION_DRIVE_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_POSITION_DRIVE_UPD({ dto.action, 0x01 });
}

void AcuDriver::TCC_ACU_OFFSET_VALUE_CMD(const Message& msg)
{
    TCC_ACU_OFFSET_VALUE_CMD_DTO dto = msg.pdu.getData<TCC_ACU_OFFSET_VALUE_CMD_DTO>();
    Timer::wait(400);
    ACU_TCC_OFFSET_VALUE_REF({ dto.axis, 0x01 });
}
