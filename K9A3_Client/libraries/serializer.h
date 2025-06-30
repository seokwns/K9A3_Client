#pragma once
#include <vector>
#include <stdlib.h>
#include <cstring>
#include "messages.h"

using serialized_data = std::pair<uint8_t, std::vector<uint8_t>>;

namespace Serializer
{
    /*
    template <typename... Args>
    static serialized_data serializeData(Args... args)
    {
        // 파라미터 개수에 따라 필요한 크기 계산
        size_t total_size = sizeof...(args) * sizeof(uint8_t);
        std::vector<uint8_t> data = { static_cast<uint8_t>(args)... };

        return std::make_pair(static_cast<uint8_t>(total_size), data);
    }
    */

    template <typename T>
    void appendToBytes(std::vector<uint8_t> &bytes, const T &value)
    {
        for (int i = sizeof(T) - 1; i >= 0; --i)
        {
            bytes.push_back(uint8_t((reinterpret_cast<const uint8_t *>(&value))[i]));
        }
    }

    template <typename... Args>
    static serialized_data serializeData(const Args &...args)
    {
        std::vector<uint8_t> bytes;
        (appendToBytes(bytes, args), ...);

        return std::make_pair(static_cast<uint8_t>(bytes.size()), bytes);
    }
}