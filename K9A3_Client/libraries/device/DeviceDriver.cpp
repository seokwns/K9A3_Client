#include "./DeviceDriver.h"

DeviceClient DeviceDriver::getClient() const
{
    return client;
}

uint8_t DeviceDriver::getDeviceId() const
{
    return deviceId;
}

int DeviceDriver::getPBitData(size_t byteIndex, size_t bitIndex)
{
    return pbit.get(byteIndex, bitIndex);
}

int DeviceDriver::getCBitData(size_t byteIndex, size_t bitIndex)
{
    return cbit.get(byteIndex, bitIndex);
}

int DeviceDriver::getIBitData(size_t byteIndex, size_t bitIndex)
{
    return ibit.get(byteIndex, bitIndex);
}

ProtocolDataUnit DeviceDriver::createPayload(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount, uint8_t dataLength, const void* data)
{
    ProtocolDataUnit pdu;
    MessageHeader header;

    header.sourceId = deviceId;
    header.destId = DeviceTable::TCC.deviceId;
    header.code = code;
    header.ack = ack;
    header.priority = priority;
    header.sendCount = sendCount;
    header.dataLength = dataLength;

    pdu.header = header;

    pdu.set_data(dataLength, data);

    return pdu;
}

ProtocolDataUnit DeviceDriver::createPayload(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount)
{
    return createPayload(code, ack, priority, sendCount, 0, nullptr);
}

ProtocolDataUnit DeviceDriver::sendMessage(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount, uint8_t dataLength, const void* data)
{
    ProtocolDataUnit pdu = createPayload(code, ack, priority, sendCount, dataLength, data);
    TurretControl::getInstance().sendPacket(client, pdu);
    return pdu;
}

ProtocolDataUnit DeviceDriver::sendMessage(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount)
{
    ProtocolDataUnit pdu = createPayload(code, ack, priority, sendCount, 0, nullptr);
    TurretControl::getInstance().sendPacket(client, pdu);
    return pdu;
}

ProtocolDataUnit DeviceDriver::sendMessage(uint8_t code, uint8_t ack)
{
    ProtocolDataUnit pdu = createPayload(code, ack, 0x02, 1, 0, nullptr);
    TurretControl::getInstance().sendPacket(client, pdu);
    return pdu;
}

ProtocolDataUnit DeviceDriver::sendMessage(uint8_t code, uint8_t ack, uint8_t dataLength, const void* data)
{
    ProtocolDataUnit pdu = createPayload(code, ack, 0x02, 1, dataLength, data);
    TurretControl::getInstance().sendPacket(client, pdu);
    return pdu;
}

void DeviceDriver::sendMessage(ProtocolDataUnit pdu)
{
    TurretControl::getInstance().sendPacket(client, pdu);
}

void DeviceDriver::BOOTDONE_UPD()
{
    sendMessage(MessageCode::BOOT_DONE, AckCode::REQUEST_ACK);
}

void DeviceDriver::SHUTDOWN_READY_REQ(const Message& msg)
{
    SHUTDOWN_READY_DONE_UPD(0x01);
}

void DeviceDriver::SHUTDOWN_READY_DONE_UPD(uint16_t result)
{
    sendMessage(MessageCode::SHUTDOWN_READY, AckCode::REQUEST_ACK, 2, &result);
}

void DeviceDriver::PBIT_RESULT_REQ(const Message& msg)
{
    PBIT_RESULT_REF();
}

void DeviceDriver::PBIT_RESULT_REF()
{
    // �׽�Ʈ�� pbit data ����
    pbit.set({ 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0 });
    sendMessage(MessageCode::PBIT_RESULT, AckCode::NO_ACK, pbit.getDataLength(), &pbit.data[0]);
}

void DeviceDriver::CBIT_SET_UPD(const Message& msg)
{
    if (connected == false)
    {
        connected = true;
    }

    uint16_t period = *reinterpret_cast<uint16_t*>(msg.pdu.data);
    cbitPeriod = period;
    Logger::getInstance().log("Device 0x%02X CBIT period set to %d ms.", deviceId, cbitPeriod);
}

void DeviceDriver::CBIT_RESULT_REQ(const Message& msg)
{
    CBIT_RESULT_REF();
}

void DeviceDriver::CBIT_RESULT_REF()
{
    // �׽�Ʈ�� cbit data ����
    Timer::wait(50);
    cbit.set({ 0, 0, 0, 0 });
    sendMessage(MessageCode::CBIT_RESULT, AckCode::NO_ACK, cbit.getDataLength(), &cbit.data[0]);
}

void DeviceDriver::IBIT_RUN_CMD(const Message& msg)
{
    IBIT_DONE_UPD();
}

void DeviceDriver::IBIT_DONE_UPD()
{
    uint16_t data = 0xFF;
    sendMessage(MessageCode::IBIT_RUN, AckCode::REQUEST_ACK, 2, &data);
}

void DeviceDriver::IBIT_RESULT_REQ(const Message& msg)
{
    IBIT_RESULT_REF();
}

void DeviceDriver::IBIT_RESULT_REF()
{
}

void DeviceDriver::EMERGENCY_CTRL_CMD(const Message& msg)
{
}

void DeviceDriver::PAUSE_CTRL_CMD(const Message& msg)
{
}

void DeviceDriver::EMERGENCY_CTRL_REF()
{
}

void DeviceDriver::PAUSE_CTRL_REF()
{
}

void DeviceDriver::STATE_INFO_REQ(const Message& msg)
{
    state_info_ref();
}