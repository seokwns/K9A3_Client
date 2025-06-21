#include "./TurretControl.h"

/**
 * ============================ 생성자 ============================
 */
TurretControl::TurretControl() : deviceId(DeviceTable::TCC.deviceId), port(DeviceTable::TCC.port), running(true)
{
    if (!ackThread)
    {
        ackThread = std::make_unique<std::thread>(&TurretControl::AckWorker, this);
    }

    if (!serverThread)
    {
        serverThread = std::make_unique<std::thread>(&TurretControl::EthernetWorker, this);
        std::cout << "[TCC] Client start (Thread ID: " << serverThread->get_id() << ")" << std::endl;
    }
}

/**
 * ============================ 소멸자 ============================
 */
TurretControl::~TurretControl()
{
    running = false;

    if (ackThread)
    {
        ackThread->join();
        ackThread = nullptr;
    }

    if (serverThread)
    {
        serverThread->join();
        std::cout << "[TCC] Client stop (Thread ID: " << serverThread->get_id() << ")" << std::endl;
        serverThread = nullptr;
    }
}

/**
 * ============================ Getter ============================
 */
uint8_t TurretControl::getDeviceId() const
{
    return deviceId;
}

bool TurretControl::getBootdoneState(uint8_t _deviceId) const
{
    auto it = bootDone.find(_deviceId);
    return (it != bootDone.end()) ? it->second : false;
}

bool TurretControl::getEmergencyCtrlState(uint8_t _deviceId) const
{
    auto it = emergencyCtrl.find(_deviceId);
    return (it != emergencyCtrl.end()) ? it->second : false;
}

/**
 * ============================ Setter ============================
 */
void TurretControl::setBootdone(uint8_t _device_id, bool state)
{
    bootDone[_device_id] = state;
}

void TurretControl::setShutdownReadyDone(uint8_t _device_id, bool state)
{
    shutdownReadyDone[_device_id] = state;
}

void TurretControl::setEmergencyCtrlState(uint8_t _deviceId, uint16_t result)
{
    emergencyCtrl[_deviceId] = result;
}

void TurretControl::setPauseCtrlState(uint8_t _deviceId, uint16_t result)
{
    pauseCtrl[_deviceId] = result;
}

/**
 * ============================ 데이터 파싱 ============================
 */
DeviceClient TurretControl::createClient(const sockaddr_in &client_addr)
{
    return {inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)};
}

std::optional<ProtocolDataUnit> TurretControl::parsePayload(const uint8_t *buffer, size_t recv_len)
{
    MessageHeader header;
    std::memcpy(&header, buffer, sizeof(header));

    if (header.dataLength < recv_len - 14)
    {
        header.print();
        std::cerr << std::endl
                  << "[Server Thread] ProtocolDataUnit data length exceeds maximum allowed size. (header: " << static_cast<int>(header.dataLength) << ", actual: " << recv_len - 12 << ")" << std::endl;
        return {};
    }

    ProtocolDataUnit pdu(header);
    std::memcpy(pdu.data, buffer + HEADER_SIZE, header.dataLength);
    std::memcpy(&pdu.checksum, buffer + HEADER_SIZE + header.dataLength, CHECKSUM_SIZE);

    return pdu;
}

uint8_t *TurretControl::serializePDU(const ProtocolDataUnit &pdu, size_t total_size)
{
    uint8_t *serialized_pdu = new uint8_t[total_size];
    std::memcpy(serialized_pdu, &pdu.header, HEADER_SIZE);

    if ((pdu.data != nullptr) && (pdu.header.dataLength > 0))
    {
        std::memcpy(serialized_pdu + HEADER_SIZE, pdu.data, pdu.header.dataLength);
    }

    std::memcpy(serialized_pdu + HEADER_SIZE + pdu.header.dataLength, &pdu.checksum, CHECKSUM_SIZE);
    return serialized_pdu;
}

/**
 * ============================ UDP 데이터 수신부 ============================
 */
void TurretControl::EthernetWorker()
{
    pool = std::make_unique<ThreadPool>(server::THREAD_POOL_SIZE, [&](const Message &msg)
                                        { handlePacketData(msg); });

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "[Server Thread] Socket creation failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9003);

    if (bind(sock, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
    {
        std::cerr << "[Server Thread] Error: Socket bind failed." << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }
    fd_set read_fds;
    int max_fd = sock;

    while (running)
    {
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        int activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (activity < 0)
        {
            std::cerr << "Select error" << std::endl;
            continue;
        }

        if (FD_ISSET(sock, &read_fds))
        {
            unsigned char buffer[server::BUFFER_SIZE] = {0};
            sockaddr_in client_addr = {};
            socklen_t client_len = sizeof(client_addr);

            ssize_t recv_len = recvfrom(sock, buffer, server::BUFFER_SIZE - 1, 0,
                                        reinterpret_cast<sockaddr *>(&client_addr), &client_len);

            if (recv_len > 0)
            {
                DeviceClient client = createClient(client_addr);
                auto pdu = parsePayload(buffer, recv_len);

                if (!pdu)
                {
                    continue;
                }

                // std::cout << client.ip << ":" << client.port << std::endl;

                Message msg{client, *pdu};
                pool->enqueue(msg);
            }
        }
    }

    close(sock);
}

/**
 * ============================ UDP 데이터 처리 ============================
 */
void TurretControl::handlePacketData(const Message &msg)
{
    auto _device = getDevice(msg.pdu.header.destId);

    // 등록되지 않은 장치에서 메시지가 올 경우 종료
    if (!_device)
    {
        msg.pdu.print();
        printf("[TCC] Error: No device matched | device id: 0x%02X\n", msg.pdu.header.sourceId);
        return;
    }

    // 체크섬 에러 시 종료
    if (msg.pdu.checksum != ProtocolDataUnit::calculateChecksum(msg.pdu))
    {
        // ACK를 필요로 하는 메시지의 경우 NACK 전송
        if (msg.pdu.header.ack == AckCode::REQUEST_ACK)
        {
            sendAckMessage(_device->getClient(), msg.pdu.header.sourceId, msg.pdu.header.code, AckCode::NACK, AckData::CHECKSUM_ERR);
        }

        msg.pdu.print();
        Logger::getInstance().error(
            "[TCC] Error: Checksum mismatch for device 0x%02X | expected: 0x%04X, received: 0x%04X",
            msg.pdu.header.sourceId, ProtocolDataUnit::calculateChecksum(msg.pdu), msg.pdu.checksum);

        sendAckMessage(tccClient, _device->getDeviceId(), msg.pdu.header.code, AckCode::NACK, AckData::CHECKSUM_ERR);

        return;
    }

    // 페이로드 데이터일 경우
    if (msg.pdu.header.isPayload() == true)
    {
        // ACK를 필요로 하는 메시지의 경우 ACK 전송 후 데이터 처리
        if (msg.pdu.header.ack == AckCode::REQUEST_ACK)
        {
            sendAckMessage(tccClient, _device->getDeviceId(), msg.pdu.header.code, AckCode::ACK, AckData::ACK_SUCCESS);
        }
        _device->handleMessage(msg);
    }
    // ACK 데이터일 경우
    else if (msg.pdu.header.isAck() == true)
    {
        handleAckData(msg);
    }
}

/**
 * ============================ UDP 데이터 전송 ============================
 */
void TurretControl::sendPacket(const DeviceClient &client, ProtocolDataUnit &pdu)
{
    // pdu.header.setTimestamp();
    pdu.header.timestamp = 0;
    pdu.setChecksum();

    //const uint8_t* header_bytes = reinterpret_cast<const uint8_t*>(&pdu.header);
    //for (size_t i = 0; i < sizeof(MessageHeader); ++i)
    //{
    //    std::cout << std::hex << std::setw(2) << static_cast<int>(header_bytes[i]) << " ";
    //}

    //// pdu.data 바이트 단위로 읽기
    //for (size_t i = 0; i < pdu.header.dataLength; ++i)
    //{
    //    std::cout << std::hex << std::setw(2) << static_cast<int>(pdu.data[i]) << " ";
    //}
    //std::cout << std::endl;
    //
    //Logger::getInstance().log("checksum: 0x%04X", pdu.checksum);


    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "[Server Thread] Error: Socket creation failed" << std::endl;
        return;
    }

    sockaddr_in client_addr{};
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(tccClient.ip.c_str());
    client_addr.sin_port = htons(tccClient.port);

    size_t total_size = HEADER_SIZE + pdu.header.dataLength + CHECKSUM_SIZE;
    uint8_t *serialized_pdu = serializePDU(pdu, total_size);

    ssize_t sent_len = sendto(sock, serialized_pdu, total_size, 0,
                              reinterpret_cast<sockaddr *>(&client_addr), sizeof(client_addr));

    if (sent_len < 0)
    {
        std::cerr << "[Server Thread] Error: Message sending failed to " << client.ip << ":" << client.port << std::endl;
    }

    delete[] serialized_pdu;
    close(sock);

    if (pdu.header.ack == AckCode::REQUEST_ACK)
    {
        msgList.push_back(pdu);
    }
}

/**
 * ============================ ACK 데이터 처리 ============================
 */

/**
 * @brief 현재 시간(Unix epoch) 기준 milliseconds 단위 4byte timestamp 반환
 */
uint32_t TurretControl::getCurrentTimestamp()
{
    auto millisec_since_epoch = duration_cast<milliseconds>(
                                    system_clock::now().time_since_epoch())
                                    .count();

    return static_cast<uint32_t>(millisec_since_epoch);
}

void TurretControl::AckWorker()
{
    while (running)
    {
        const auto now = getCurrentTimestamp();

        for (auto itr = msgList.begin(); itr != msgList.end();)
        {
            const auto elapsed = now - itr->header.timestamp;

            if (elapsed <= protocol::TIMEOUT_MS)
            {
                ++itr;
                continue;
            }

            if (itr->header.sendCount > protocol::MAX_RETRANSMIT)
            {
                itr = msgList.erase(itr);

                // TODO: 최대 재전송 횟수 초과 시 비상정지

                continue;
            }

            auto devicePtr = getDevice(itr->header.destId);
            if (devicePtr)
            {
                ProtocolDataUnit _pdu = *itr;
                itr = msgList.erase(itr);
                _pdu.header.sendCount++;
                devicePtr->sendMessage(_pdu);

                std::cout << std::endl
                          << "[ACK Thread] ACK not received. Retrying "
                          << "0x" << std::uppercase << std::hex << static_cast<int>(itr->header.sourceId)
                          << static_cast<int>(itr->header.destId) << static_cast<int>(itr->header.code) << std::setw(2) << std::setfill('0') << static_cast<int>(itr->header.ack)
                          << std::dec << " (" << static_cast<int>(itr->header.sendCount) << ")" << std::endl;
            }

            ++itr;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TurretControl::sendAckMessage(const DeviceClient &client, uint8_t destId, uint8_t code, uint8_t ack_result, uint8_t ack_info)
{
    MessageHeader ack_header{};
    ack_header.priority = 0x02;
    ack_header.sourceId = destId;
    ack_header.destId = DeviceTable::TCC.deviceId;
    ack_header.code = code;
    ack_header.ack = ack_result;
    ack_header.sendCount = 1;
    ack_header.dataLength = 4;

    ProtocolDataUnit ack_payload(ack_header);
    AckPayloadData dto(code, ack_info);
    ack_payload.set_data(&dto, sizeof(dto));

    sendPacket(client, ack_payload);
}

void TurretControl::handleAckData(const Message &msg)
{
    auto itr = std::find_if(msgList.begin(), msgList.end(),
                            [this, msg](const ProtocolDataUnit &pdu)
                            {
                                return pdu.header.sourceId == msg.pdu.header.sourceId && pdu.header.destId == deviceId && pdu.header.code == msg.pdu.header.code && pdu.header.ack == msg.pdu.header.ack && pdu.header.timestamp == msg.pdu.header.timestamp;
                            });

    if (itr == msgList.end())
    {
        return;
    }

    msgList.erase(itr);
}