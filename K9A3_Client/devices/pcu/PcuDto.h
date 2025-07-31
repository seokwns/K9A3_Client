#pragma once

#include <cstdint>

#pragma pack(push, 1)
// PCU > TCC
struct PCU_TCC_ENCODER_RESET_RESULT_REF_DTO
{
    uint16_t encoder;
    uint16_t result;
};

struct PCU_TCC_VALIDATION_RESULT_UPD_DTO
{
    uint16_t command;
    uint16_t axis;
    uint16_t result;
};

struct PCU_TCC_INSERT_RESULT_UPD_DTO
{
    uint16_t result;
};

struct PCU_TCC_SINGLE_DRIVE_UPD_DTO
{
    uint16_t axis;
    uint16_t action;
    uint16_t sign;
    uint16_t data;
    uint16_t result;
};

struct PCU_TCC_SEQUENCE_DRIVE_UPD_DTO
{
    uint16_t seq;
    uint16_t endSeq;
    uint16_t loaded;
    uint16_t slope;
    uint16_t vehicleTilt;
    uint16_t result;
};

struct PCU_TCC_POSITION_DRIVE_UPD_DTO
{
    uint16_t command;
    uint16_t result;
};

// TCC > PCU
struct TCC_PCU_RESET_UPD_DTO
{
    uint16_t target;
};

struct TCC_PCU_ENCODER_RESET_RESULT_REQ_DTO
{
    uint16_t target;
};

struct TCC_PCU_ENCODER_RESET_REQ_DTO
{
    uint16_t target;
};

struct TCC_PCU_BRAKE_CTRL_UPD_DTO
{
    uint16_t action;
    uint16_t target;
};

struct TCC_PCU_VALIDATION_CMD_DTO
{
    uint16_t action;
    uint16_t axis;
    uint16_t speed;
};

struct TCC_PCU_SINGLE_DRIVE_CMD_DTO
{
    uint16_t axis;
    uint16_t action;
    uint16_t sign;
    int16_t data;
    uint16_t speed;
};

struct TCC_PCU_SEQUENCE_DRIVE_CMD_DTO
{
    uint16_t seq;
    uint16_t startSeq;
    uint16_t loaded;
    uint16_t slope;
    uint16_t vehicleTilt;
    uint16_t speed;
};

struct TCC_PCU_POSITION_DRIVE_CMD_DTO
{
    uint16_t action;
    uint16_t speed;
};
#pragma pack(pop)