#pragma once

// Standard Library
#include <arpa/inet.h>
#include <atomic>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <signal.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <optional>

// Project Headers
#include "../singleton.h"
#include "../constants/constants.h"
#include "../constants/DeviceConstants.h"
#include "../messages.h"
#include "../serializer.h"
#include "../threads/ThreadPool.h"
#include "./TccState.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

class DeviceDriver;

class TurretControl : public Singleton<TurretControl>
{
private:
    // Member variables
    const uint8_t deviceId;
    const uint16_t port;
    const DeviceClient tccClient = { DeviceTable::TCC.ip, DeviceTable::TCC.port };

    TccState state;

    std::unordered_map<uint8_t, std::shared_ptr<DeviceDriver>> devices;
    std::unordered_map<uint8_t, bool> bootDone;
    std::unordered_map<uint8_t, uint16_t> shutdownReadyDone;
    std::unordered_map<uint8_t, uint16_t> emergencyCtrl;
    std::unordered_map<uint8_t, uint16_t> pauseCtrl;

    bool running = false;
    int sock = -1;

    std::vector<ProtocolDataUnit> msgList;

    std::unique_ptr<ThreadPool> pool;
    std::unique_ptr<std::thread> ackThread;
    std::unique_ptr<std::thread> serverThread;
    std::unique_ptr<std::thread> cbitThread;

    std::vector<std::shared_ptr<DeviceDriver>> deviceList;

    // Helper functions
    DeviceClient createClient(const sockaddr_in& client_addr);
    std::optional<ProtocolDataUnit> parsePayload(const uint8_t* buffer, size_t recv_len);
    uint8_t* serializePDU(const ProtocolDataUnit& pdu, size_t total_size);

    /**
     * @brief ACK 수신 여부를 체크 후 재전송하는 스레드 worker
     */
    void AckWorker();

    /**
     * @brief UDP 패킷 수신 스레드 worker
     */
    void EthernetWorker();

    void cbitWorker();

public:
    // Constructor & Destructor
    TurretControl();
    ~TurretControl();

    void init();

    /**
     * @brief 연동 장치 추가
     *
     * @param _device DeviceDriver를 상속한 장치 클래스
     */
    template <typename T>
    void addDevice(T* _device);

    /**
     * @brief 파라미터로 전달받은 모든 장치들을 추가합니다
     *
     * @param args 장치 포인터
     */
    template <typename... Args>
    void addDevices(Args... args);

    /**
     * @brief 화포통제컴퓨터의 장치 ID 반환
     *
     * @return ID
     */
    uint8_t getDeviceId() const;

    /**
     * @brief ID에 매핑된 장치 포인터를 반환합니다.
     *
     * @param _deviceId 장치 ID
     * @return 장치 클래스 포인터
     */
    std::shared_ptr<DeviceDriver> getDevice(uint8_t _deviceId) const;

    /**
     * @brief 해당 장치의 boot done 상태를 반환합니다.
     *
     * @param _deviceId 장치 ID
     * @return boot-done 상태
     */
    bool getBootdoneState(uint8_t _deviceId) const;

    /**
     * @brief 해당 장치의 비상정지 상태를 반환합니다.
     *
     * @param _deviceId 장치 ID
     * @return 비상정지 완료 여부
     */
    bool getEmergencyCtrlState(uint8_t _deviceId) const;

    /**
     * @brief 현재 시간을 반환합니다. (milli second는 8바이트 데이터지만, 하위 4바이트만 사용합니다.)
     *
     * @return 현재시간 (milli second)
     */
    uint32_t getCurrentTimestamp();

    // Setter
    void setBootdone(uint8_t _deviceId, bool state);
    void setShutdownReadyDone(uint8_t _deviceId, bool state);
    void setPauseCtrlState(uint8_t _deviceId, uint16_t result);
    void setEmergencyCtrlState(uint8_t _deviceId, uint16_t result);

    // Network and packet handling
    void sendPacket(const DeviceClient& client, ProtocolDataUnit& pdu);
    void handlePacketData(const Message& msg);
    void sendAckMessage(const DeviceClient& client, uint8_t destId, uint8_t code, uint8_t ack_result, uint8_t ack_info);
    void handleAckData(const Message& msg);
};

template <typename T>
inline void TurretControl::addDevice(T* _device)
{
    static_assert(std::is_base_of<DeviceDriver, T>::value, "[TCC] Error: T must be derived from device");
    auto dev_ptr = std::shared_ptr<T>(_device);

    // TODO: 장치 IP-PORT 확인
    // TODO: 운용통제장치의 경우, 확인 후 권한 이양

    devices[_device->getDeviceId()] = dev_ptr;
    deviceList.push_back(dev_ptr);

    // _device->init();
}

template <typename... Args>
inline void TurretControl::addDevices(Args... args)
{
    (addDevice(args), ...);
}

inline std::shared_ptr<DeviceDriver> TurretControl::getDevice(uint8_t _deviceId) const
{
    auto it = devices.find(_deviceId);
    return (it != devices.end()) ? it->second : nullptr;
}

#include "../device/DeviceDriver.h"