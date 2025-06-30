#pragma once

#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../messages.h"
#include "../tcc/TurretControl.h"
#include "./DeviceState.h"
#include "./BitData.h"
#include "../constants/DeviceConstants.h"
#include "../Timer.h"
#include "../Logger.h"

using MessageHandler = std::function<void(const Message&)>;

class DeviceDriver
{
protected:
    const uint8_t deviceId;
    const DeviceClient client;

    uint16_t cbitPeriod;

    BitData pbit;
    BitData cbit;
    BitData ibit;

    bool connected = false;

    std::unordered_map<uint8_t, MessageHandler> messageHandlerMapper;

public:
    // 생성자
    DeviceDriver(DeviceRecord record) : deviceId(record.deviceId), client({ record.ip, record.port })
    {
        addMessageHandler(MessageCode::SHUTDOWN_READY, &DeviceDriver::SHUTDOWN_READY_REQ);
        addMessageHandler(MessageCode::PBIT_RESULT, &DeviceDriver::PBIT_RESULT_REQ);
        addMessageHandler(MessageCode::CBIT_SET, &DeviceDriver::CBIT_SET_UPD);
        addMessageHandler(MessageCode::CBIT_RESULT, &DeviceDriver::CBIT_RESULT_REQ);
        addMessageHandler(MessageCode::IBIT_RUN, &DeviceDriver::IBIT_RUN_CMD);
        addMessageHandler(MessageCode::IBIT_RESULT, &DeviceDriver::IBIT_RESULT_REQ);
        addMessageHandler(MessageCode::EMERGENCY_CTRL, &DeviceDriver::PAUSE_CTRL_CMD);
        addMessageHandler(MessageCode::PAUSE_CTRL, &DeviceDriver::PAUSE_CTRL_CMD);
        addMessageHandler(MessageCode::STATE_INFO, &DeviceDriver::STATE_INFO_REQ);

        BOOTDONE_UPD();
    }

    /**
     * @brief 디바이스 초기화 (필요시 오버라이드)
     */
    virtual void init() {}

    /**
     * @brief 디바이스 정보 출력 (필요시 오버라이드)
     */
    virtual void print() {}

    /**
     * @brief 메시지 처리 시도
     *
     * @param msg 메시지 객체
     * @return true  처리 성공
     * @return false 핸들러 없음
     */
    bool handleMessage(const Message& msg)
    {
        auto it = messageHandlerMapper.find(msg.pdu.header.code);
        if (it != messageHandlerMapper.end())
        {
            it->second(msg);
            return true;
        }
        else
        {
            std::cerr << "[Device] Error: No handler found" << std::endl;
            msg.pdu.print();
            return false;
        }
    }

    DeviceClient getClient() const;

    uint8_t getDeviceId() const;

    bool isConnected() const { return connected; }

    virtual DeviceState* getDeviceState() const {}

    uint16_t getCBitPeriod() const { return cbitPeriod; }

    int getPBitData(size_t byteIndex, size_t bitIndex);

    int getCBitData(size_t byteIndex, size_t bitIndex);

    int getIBitData(size_t byteIndex, size_t bitIndex);

    ProtocolDataUnit createPayload(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount, uint8_t dataLength, const void* data);
    ProtocolDataUnit createPayload(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount);

    ProtocolDataUnit sendMessage(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount, uint8_t dataLength, const void* data);
    ProtocolDataUnit sendMessage(uint8_t code, uint8_t ack, uint8_t dataLength, const void* data);
    ProtocolDataUnit sendMessage(uint8_t code, uint8_t ack, uint8_t priority, uint8_t sendCount);
    ProtocolDataUnit sendMessage(uint8_t code, uint8_t ack);
    void sendMessage(ProtocolDataUnit pdu);

    void BOOTDONE_UPD();

    void SHUTDOWN_READY_REQ(const Message& msg);
    void SHUTDOWN_READY_DONE_UPD(uint16_t result);

    void PBIT_RESULT_REQ(const Message& msg);
    void PBIT_RESULT_REF();

    void CBIT_SET_UPD(const Message& msg);
    void CBIT_RESULT_REQ(const Message& msg);
    void CBIT_RESULT_REF();

    void IBIT_RUN_CMD(const Message& msg);
    void IBIT_DONE_UPD();
    void IBIT_RESULT_REQ(const Message& msg);
    void IBIT_RESULT_REF();

    void EMERGENCY_CTRL_CMD(const Message& msg);
    void EMERGENCY_CTRL_REF();

    void PAUSE_CTRL_CMD(const Message& msg);
    void PAUSE_CTRL_REF();

    void STATE_INFO_REQ(const Message& msg);
    virtual void state_info_ref() {};

protected:
    /**
     * @brief 메시지 핸들러 등록 템플릿
     *
     * @param code 메시지 코드
     * @param ack ACK 코드
     * @param method 멤버 함수 포인터
     * @param T this 타입 (device 또는 파생 클래스)
     */
    template <typename T>
    void addMessageHandler(uint8_t code, void (T::* method)(const Message&))
    {
        messageHandlerMapper[code] = [this, method](const Message& msg)
            {
                (static_cast<T*>(this)->*method)(msg);
            };
    }
};