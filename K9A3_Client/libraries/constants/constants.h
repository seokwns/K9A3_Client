#pragma once

#include <cstring>

namespace server
{
    /**
     * @brief 최대 버퍼 크기 설정
     */
    constexpr int BUFFER_SIZE = 1024;

    /**
     * @brief 데이터를 수신할 스레드 풀 개수 설정
     */
    constexpr int THREAD_POOL_SIZE = 20;
}

namespace protocol
{
    /*
     * @brief 최대 재전송 횟수 설정
     */
    constexpr int MAX_RETRANSMIT = 4;

    /*
     * @brief ACK 대기 타임아웃 설정 (ms)
     */
    constexpr uint32_t TIMEOUT_MS = 100;

    /**
     * @brief 상태정보 재전송 간격 설정 (ms)
     */
    constexpr int STATE_MSG_PERIOD = 100;
}