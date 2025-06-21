#pragma once

#include <iostream>
#include <cstdint>
#include <array>
#include <map>
#include <cstring>
#include <vector>
#include <algorithm>

struct BitData
{
	bool state;
	std::vector<uint8_t> data;

	BitData() : state(false), data{} {}

	uint8_t getDataLength() const
	{
		return static_cast<uint8_t>(data.size());
	}

	/**
	 * @brief 해당 위치의 비트 데이터를 설정합니다.
	 *
	 * @param byteIndex 바이트 인덱스
	 * @param bitIndex 비트 인덱스
	 */
	void set(const std::vector<uint8_t> &byteData)
	{
		/*
		if (byteIndex + byteData.size() > data.size())
		{
			std::cerr << std::endl << "[BitField] Error: Invalid segment indices." << std::endl;
			return;
		}

		std::copy(byteData.begin(), byteData.end(), data.begin() + byteIndex);
		*/
		data = std::vector<uint8_t>(byteData.begin(), byteData.end());
	}

	/**
	 * @brief 해당 위치의 비트 데이터를 반환합니다.
	 *
	 * @param byteIndex 바이트 인덱스
	 * @param bitIndex 비트 인덱스
	 * @return 해당 위치의 바이트 데이터를 10진수로 변환한 값
	 */
	int get(size_t byteIndex, size_t bitIndex) const
	{
		if (byteIndex >= data.size())
		{
			return false;
		}

		uint8_t temp = data[byteIndex];
		int result = ((temp >> (bitIndex * 2)) & 0x03);

		return result;
	}

	/**
	 * @brief 고장 여부를 체크합니다.
	 *
	 * @return 고장여부
	 */
	bool hasError()
	{
		for (uint8_t item : data)
		{
			// BIT 데이터에서 고장 조건: unsigned char(uint8_t) 데이터가 0보다 크고, 2의 배수면 고장 그 외에는 경고 상태
			if ((item > 0) && (item % 2 == 0))
				return true;
		}

		return false;
	}
};