#include "./MccuDriver.h"


void MccuDriver::state_info_ref()
{
    sendMessage(MessageCode::STATE_INFO, AckCode::NO_ACK, static_cast<uint8_t>(sizeof(*state)), state);
}

void MccuDriver::MCCU_TCC_ENCODER_RESET_RESULT_REF(MCCU_TCC_ENCODER_RESET_RESULT_REF_DTO dto)
{
    sendMessage(0x21, AckCode::NO_ACK, sizeof(dto), &dto);
}

void MccuDriver::MCCU_TCC_VALIDATION_RESULT_UPD(MCCU_TCC_VALIDATION_RESULT_UPD_DTO dto)
{
    sendMessage(0x32, AckCode::NO_ACK, sizeof(dto), &dto);
}

void MccuDriver::MCCU_TCC_SINGLE_DRIVE_UPD(MCCU_TCC_SINGLE_DRIVE_UPD_DTO dto)
{
    sendMessage(0x40, AckCode::NO_ACK, sizeof(dto), &dto);
}

void MccuDriver::MCCU_TCC_SEQUENCE_DRIVE_UPD(MCCU_TCC_SEQUENCE_DRIVE_UPD_DTO dto)
{
    sendMessage(0x41, AckCode::NO_ACK, sizeof(dto), &dto);
}

void MccuDriver::MCCU_TCC_POSITION_DRIVE_UPD(MCCU_TCC_POSITION_DRIVE_UPD_DTO dto)
{
    sendMessage(0x42, AckCode::NO_ACK, sizeof(dto), &dto);
}

void MccuDriver::TCC_MCCU_RESET_UPD(const Message& msg)
{
    
}

void MccuDriver::TCC_MCCU_ENCODER_RESET_REQ(const Message& msg)
{
    TCC_MCCU_ENCODER_RESET_REQ_DTO dto = msg.pdu.getData<TCC_MCCU_ENCODER_RESET_REQ_DTO>();
    Timer::wait(400);
    MCCU_TCC_ENCODER_RESET_RESULT_REF({ dto.target, 0x01 });
}

void MccuDriver::TCC_MCCU_BRAKE_CTRL_UPD(const Message& msg)
{
}

void MccuDriver::TCC_MCCU_VALIDATION_CMD(const Message& msg)
{
    TCC_MCCU_VALIDATION_CMD_DTO dto = msg.pdu.getData<TCC_MCCU_VALIDATION_CMD_DTO>();
    Timer::wait(400);
    MCCU_TCC_VALIDATION_RESULT_UPD({ dto.action, dto.axis, 0x01 });
}

void MccuDriver::TCC_MCCU_SINGLE_DRIVE_CMD(const Message& msg)
{
    TCC_MCCU_SINGLE_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_MCCU_SINGLE_DRIVE_CMD_DTO>();
    Timer::wait(400);
    MCCU_TCC_SINGLE_DRIVE_UPD({ dto.axis, dto.action, dto.sign, static_cast<uint16_t>(dto.data), 0x01 });
}

void MccuDriver::TCC_MCCU_SEQUENCE_DRIVE_CMD(const Message& msg)
{
    TCC_MCCU_SEQUENCE_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_MCCU_SEQUENCE_DRIVE_CMD_DTO>();
    Timer::wait(400);
    MCCU_TCC_SEQUENCE_DRIVE_UPD({ dto.seq, dto.startSeq, dto.sign, dto.chargeLocation, dto.chargeAmount, dto.sign2, dto.vehicleTilt, 0x01 });
}

void MccuDriver::TCC_MCCU_POSITION_DRIVE_CMD(const Message& msg)
{
    TCC_MCCU_POSITION_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_MCCU_POSITION_DRIVE_CMD_DTO>();
    Timer::wait(400);
    MCCU_TCC_POSITION_DRIVE_UPD({ dto.action, 0x01 });
}
