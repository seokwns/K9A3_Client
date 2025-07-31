#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct WCU_TCC_VALIDATION_RESULT_UPD_DTO
{
    uint16_t command;
    uint16_t axis;
    uint16_t result;
};

struct WCU_TCC_RECOIL_TIME_REF_DTO
{
    uint16_t time;
};

struct WCU_TCC_GUN_BREECH_CTRL_UPD_DTO
{
    uint16_t action;
    uint16_t result;
};

struct WCU_TCC_FIRE_RESULT_REF_DTO
{
    uint16_t result;
    uint16_t breechState;
};

struct TCC_WCU_RESET_UPD_DTO
{
    uint16_t target;
};

struct TCC_WCU_VALIDATION_CMD_DTO
{
    uint16_t command;
    uint16_t axis;
    uint16_t speed;
};

struct TCC_WCU_RECOIL_TIME_REQ_DTO
{
};

struct TCC_WCU_GUN_BREECH_CTRL_CMD_DTO
{
    uint16_t action;
};

#pragma pack(pop)