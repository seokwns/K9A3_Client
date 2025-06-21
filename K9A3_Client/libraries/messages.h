#pragma once

// Standard Library Includes
#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/time.h>

// Using declarations
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

/**
 * @brief 패킷 상수 정의
 */
constexpr size_t HEADER_SIZE = 12;
constexpr size_t CHECKSUM_SIZE = 2;
constexpr uint8_t ACK_MESSAGE_CODE = 0xFF;

/**
 * @brief 상태 및 명령 코드 정의
 */
namespace MessageCode
{
    constexpr uint8_t BOOT_DONE = 0x01;
    constexpr uint8_t SHUTDOWN_READY = 0x02;
    constexpr uint8_t PBIT_RESULT = 0x10;
    constexpr uint8_t CBIT_SET = 0x11;
    constexpr uint8_t CBIT_RESULT = 0x12;
    constexpr uint8_t IBIT_RUN = 0x13;
    constexpr uint8_t IBIT_RESULT = 0x14;
    constexpr uint8_t EMERGENCY_CTRL = 0x30;
    constexpr uint8_t PAUSE_CTRL = 0x31;
    constexpr uint8_t STATE_INFO = 0xA0;
}

/**
 * @brief Header 내 message ACK 코드 정의
 */
namespace AckCode
{
    constexpr uint8_t NO_ACK = 0xFF;
    constexpr uint8_t REQUEST_ACK = 0x01;
    constexpr uint8_t ACK = 0x10;
    constexpr uint8_t NACK = 0x11;
}

/**
 * @brief ACK 메시지 결과 코드 정의
 */
namespace AckData
{
    constexpr uint8_t ACK_SUCCESS = 0x00;
    constexpr uint8_t CHECKSUM_ERR = 0x01;
}

/**
 * @brief 4byte Timestamp mask
 */
constexpr uint32_t TIMESTAMP_MASK = 0xFFFFFFFF;

/**
 * @brief Message Header 구조체
 */
#pragma pack(push, 1)
struct MessageHeader
{
    uint32_t timestamp;
    uint8_t sourceId;
    uint8_t destId;
    uint8_t code;
    uint8_t ack;
    uint8_t priority;
    uint8_t sendCount;
    uint8_t dataLength;

    MessageHeader() = default;

    MessageHeader(uint32_t _timestamp, uint8_t _sid, uint8_t _did,
                  uint8_t _code, uint8_t _ack, uint8_t _priority,
                  uint8_t _count, uint8_t _len)
        : timestamp(_timestamp), sourceId(_sid), destId(_did), code(_code), ack(_ack),
          priority(_priority), sendCount(_count), dataLength(_len) {}

    /*
    bool operator<(const MessageHeader& other) const
    {
        if (msg_id != other.msg_id)
            return msg_id < other.msg_id;
        return timestamp < other.timestamp;
    }
    */

    /**
     * @brief 페이로드 메시지 여부 (ACK이 아니거나 ACK 요청인 경우)
     */
    bool isPayload() const
    {
        return ack == AckCode::NO_ACK || ack == AckCode::REQUEST_ACK;
    }

    /**
     * @brief ACK 메시지 여부
     */
    bool isAck() const
    {
        return ack == AckCode::ACK || ack == AckCode::NACK;
    }

    void setTimestamp()
    {
        auto millisec_since_epoch = duration_cast<milliseconds>(
                                        system_clock::now().time_since_epoch())
                                        .count();

        timestamp = static_cast<uint32_t>(millisec_since_epoch & TIMESTAMP_MASK);
    }

    void print()
    {
        std::cout << std::uppercase << std::hex << std::setw(2) << static_cast<int>(sourceId) << static_cast<int>(destId) << static_cast<int>(code) << std::setw(2) << std::setfill('0') << static_cast<int>(ack) << std::dec << std::endl;
    }
};

/**
 * @brief 프로토콜 데이터 유닛 구조체
 */
struct ProtocolDataUnit
{
    MessageHeader header;
    uint8_t *data;
    uint16_t checksum;

    ProtocolDataUnit() : data(nullptr), checksum(0) {}

    explicit ProtocolDataUnit(MessageHeader _header)
        : header(_header), data(nullptr), checksum(0)
    {
        allocateData(header.dataLength);
    }

    ~ProtocolDataUnit()
    {
        freeMemory();
    }

    ProtocolDataUnit(const ProtocolDataUnit &other)
        : header(other.header), data(nullptr), checksum(other.checksum)
    {
        if (other.data != nullptr)
        {
            allocateData(other.header.dataLength);
            std::memcpy(data, other.data, other.header.dataLength);
        }
    }

    ProtocolDataUnit &operator=(const ProtocolDataUnit &other)
    {
        if (this != &other)
        {
            freeMemory();
            header = other.header;
            checksum = other.checksum;

            if (other.data != nullptr)
            {
                allocateData(other.header.dataLength);
                std::memcpy(data, other.data, other.header.dataLength);
            }
        }
        return *this;
    }

    void freeMemory()
    {
        if (data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }
    }

    void allocateData(size_t dataLength)
    {
        freeMemory();
        data = new uint8_t[dataLength];
        header.dataLength = static_cast<uint8_t>(dataLength);
    }

    void print() const
    {
        std::cout << "\n-------------------- ProtocolDataUnit log --------------------\n"
                  << "timestamp: " << std::setw(9) << std::setfill('0') << header.timestamp
                  << "\tmsg id: 0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(header.sourceId)
                  << std::setw(2) << std::setfill('0') << static_cast<int>(header.destId)
                  << std::setw(2) << std::setfill('0') << static_cast<int>(header.code)
                  << std::setw(2) << std::setfill('0') << static_cast<int>(header.ack) << std::dec
                  << "\tpriority: " << static_cast<int>(header.priority) << std::endl
                  << "send count: " << static_cast<int>(header.sendCount)
                  << "\t\tdata length: " << static_cast<int>(header.dataLength)
                  << "\t\tchecksum: 0x" << std::hex << checksum << std::dec << std::endl;

        if (header.dataLength > 0 && data != nullptr)
        {
            std::cout << "data: ";
            for (int i = 0; i < header.dataLength; ++i)
            {
                std::cout << "0x" << std::hex << static_cast<int>(data[i]) << " " << std::dec;
            }
            std::cout << "\n";
        }

        std::cout << "--------------------------------------------------------------\n";
    }

    void set_data(const void *input, size_t input_size)
    {
        if (input_size == 0)
            return;

        allocateData(input_size);
        std::memcpy(data, input, input_size);
    }

    void setChecksum()
    {
        checksum = calculateChecksum(*this);
    }

    static uint16_t calculateChecksum(const ProtocolDataUnit &pdu)
    {
        uint32_t sum = 0;

        // pdu.header 바이트 단위로 읽기
        const uint8_t* header_bytes = reinterpret_cast<const uint8_t*>(&pdu.header);
        for (size_t i = 0; i < sizeof(MessageHeader); ++i)
        {
            sum += header_bytes[i];
        }

        // pdu.data 바이트 단위로 읽기
        for (size_t i = 0; i < pdu.header.dataLength; ++i)
        {
            sum += pdu.data[i];
        }

        return static_cast<uint16_t>(sum & 0xFFFF);
    }

    /*
    bool operator<(const ProtocolDataUnit& other) const
    {
        if (header.msg_id == other.header.msg_id)
            return header.timestamp < other.header.timestamp;
        return header.msg_id < other.header.msg_id;
    }
    */
};
#pragma pack(pop)

/**
 * @brief 장치 클라이언트 정보 구조체
 */
struct DeviceClient
{
    std::string ip;
    uint16_t port;

    DeviceClient() {}
    DeviceClient(std::string _ip, uint16_t _port) : ip(_ip), port(_port) {}
};

/**
 * @brief 메시지 구조체
 */
struct Message
{
    DeviceClient client;
    ProtocolDataUnit pdu;

    Message() {}

    Message(DeviceClient _client, ProtocolDataUnit _pdu) : client(_client), pdu(_pdu) {}

    bool operator<(const Message &other) const
    {
        // priority가 같으면 send_count가 작은 것이 우선
        if (pdu.header.priority == other.pdu.header.priority)
        {
            return pdu.header.sendCount < other.pdu.header.sendCount;
        }

        // priority가 높은 것이 우선
        return pdu.header.priority > other.pdu.header.priority;
    }
};

/**
 * @brief ACK 페이로드 데이터 구조체 (DTO)
 *   - code: 명령 코드
 *   - info: 0x00 정상 | 0x01 에러 등
 */
struct AckPayloadData
{
    uint16_t code;
    uint16_t info;

    AckPayloadData(uint16_t _code, uint16_t _info)
        : code(_code), info(_info) {}

    static AckPayloadData decode(const Message &msg)
    {
        return *reinterpret_cast<const AckPayloadData *>(msg.pdu.data);
    }
};