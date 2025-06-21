#include "./AttlaDriver.h"

void AttlaDriver::tcc_attla_reset_upd(const Message &msg) {}

void AttlaDriver::tcc_attla_encoder_reset_req(const Message &msg)
{
    msg.pdu.print();
}

void AttlaDriver::tcc_attla_drive_cmd_cancel_cmd(const Message &msg) {}

void AttlaDriver::tcc_attla_validation_cmd(const Message &msg) {}

void AttlaDriver::tcc_attla_drive_cmd(const Message &msg) {}

void AttlaDriver::attla_tcc_encoder_reset_result_ref(unsigned short target, unsigned short result)
{
    serialized_data data = Serializer::serializeData(target, result);
    struct dto
    {
        uint16_t target;
        uint16_t result;
    };

    dto _dto = {target, result};
    sendMessage(0x21, AckCode::NO_ACK, sizeof(_dto), &_dto);
}

void AttlaDriver::attla_tcc_drive_cmd_cancel_ref(unsigned short result)
{
    sendMessage(0x31, AckCode::NO_ACK, 2, &result);
}

void AttlaDriver::attla_tcc_validation_result_upd(unsigned short target, unsigned short result)
{
    serialized_data data = Serializer::serializeData(target, result);
    sendMessage(0x32, AckCode::REQUEST_ACK, data.first, &data.second);
}

void AttlaDriver::attla_tcc_drive_upd(unsigned short drive_cmd, unsigned short result)
{
    serialized_data data = Serializer::serializeData(drive_cmd, result);
    sendMessage(0x40, AckCode::REQUEST_ACK, data.first, &data.second);
}

void AttlaDriver::state_info_ref()
{
    sendMessage(MessageCode::STATE_INFO, AckCode::NO_ACK, static_cast<uint8_t>(sizeof(*state)), state);
}