#include "./PcuDriver.h"

void PcuDriver::TCC_PCU_RESET_UPD(const Message& msg)
{
}

void PcuDriver::TCC_PCU_ENCODER_RESET_REQ(const Message& msg)
{
    TCC_PCU_ENCODER_RESET_RESULT_REQ_DTO dto = msg.pdu.getData<TCC_PCU_ENCODER_RESET_RESULT_REQ_DTO>();
    PCU_TCC_ENCODER_RESET_RESULT_REF({ dto.target, 0x01 });
}

void PcuDriver::TCC_PCU_BRAKE_CTRL_UPD(const Message& msg)
{
}

void PcuDriver::TCC_PCU_VALIDATION_CMD(const Message& msg)
{
    TCC_PCU_VALIDATION_CMD_DTO dto = msg.pdu.getData<TCC_PCU_VALIDATION_CMD_DTO>();
    PCU_TCC_VALIDATION_RESULT_UPD({ dto.action, dto.axis, 0x01 });
}

void PcuDriver::TCC_PCU_SINGLE_DRIVE_CMD(const Message& msg)
{
    TCC_PCU_SINGLE_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_PCU_SINGLE_DRIVE_CMD_DTO>();
    Timer::wait(400);
    PCU_TCC_SINGLE_DRIVE_UPD({ dto.axis, dto.action, dto.sign, static_cast<uint16_t>(dto.data), 0x01 });
}

void PcuDriver::TCC_PCU_SEQUENCE_DRIVE_CMD(const Message& msg)
{
    TCC_PCU_SEQUENCE_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_PCU_SEQUENCE_DRIVE_CMD_DTO>();
    Timer::wait(400);
    PCU_TCC_SEQUENCE_DRIVE_UPD({ dto.seq, dto.startSeq, dto.loaded, dto.slope, dto.vehicleTilt, 0x01 });
}

void PcuDriver::TCC_PCU_POSITION_DRIVE_CMD(const Message& msg)
{
    TCC_PCU_POSITION_DRIVE_CMD_DTO dto = msg.pdu.getData<TCC_PCU_POSITION_DRIVE_CMD_DTO>();
    PCU_TCC_POSITION_DRIVE_UPD_DTO _dto = { dto.action, 0x01 };
    sendMessage(0x42, AckCode::NO_ACK, sizeof(_dto), &_dto);
}

void PcuDriver::state_info_ref()
{
    sendMessage(MessageCode::STATE_INFO, AckCode::NO_ACK, static_cast<uint8_t>(sizeof(*state)), state);
}

void PcuDriver::PCU_TCC_ENCODER_RESET_RESULT_REF(PCU_TCC_ENCODER_RESET_RESULT_REF_DTO dto)
{
    sendMessage(0x21, AckCode::NO_ACK, sizeof(dto), &dto);
}

void PcuDriver::PCU_TCC_VALIDATION_RESULT_UPD(PCU_TCC_VALIDATION_RESULT_UPD_DTO dto)
{
    sendMessage(0x32, AckCode::REQUEST_ACK, sizeof(dto), &dto);
}

void PcuDriver::PCU_TCC_INSERT_RESULT_UPD(PCU_TCC_INSERT_RESULT_UPD_DTO dto)
{
}

void PcuDriver::PCU_TCC_SINGLE_DRIVE_UPD(PCU_TCC_SINGLE_DRIVE_UPD_DTO dto)
{
    sendMessage(0x40, AckCode::REQUEST_ACK, sizeof(dto), &dto);
}

void PcuDriver::PCU_TCC_SEQUENCE_DRIVE_UPD(PCU_TCC_SEQUENCE_DRIVE_UPD_DTO dto)
{
    sendMessage(0x41, AckCode::REQUEST_ACK, sizeof(dto), &dto);
}

void PcuDriver::PCU_TCC_POSITION_DRIVE_UPD(PCU_TCC_POSITION_DRIVE_UPD_DTO dto)
{
    sendMessage(0x42, AckCode::NO_ACK, sizeof(dto), &dto);
}
