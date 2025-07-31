#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct ACU_TCC_ENCODER_RESET_RESULT_REF_DTO
{
    uint16_t encoder;
    uint16_t result;
};

struct ACU_TCC_VALIDATION_RESULT_UPD_DTO
{
    uint16_t command;
    uint16_t axis;
    uint16_t result;
};

struct ACU_TCC_PASS_PROJECTILE_REF_DTO
{
    uint16_t result;
};

struct ACU_TCC_PASS_CHARGE_REF_DTO
{
    uint16_t result;
};

struct ACU_TCC_SINGLE_DRIVE_UPD_DTO
{
    uint16_t axis;
    uint16_t action;
    uint16_t result;
};

struct ACU_TCC_ALIGN_GUN_BREECH_UPD_DTO
{
    uint16_t seq;
    uint16_t result;
};

struct ACU_TCC_LOAD_PROJECTILE_UPD_DTO
{
    uint16_t seq;
    uint16_t result;
};

struct ACU_TCC_LOAD_CHARGE_UPD_DTO
{
    uint16_t seq;
    uint16_t result;
};

struct ACU_TCC_POSITION_DRIVE_UPD_DTO
{
    uint16_t action;
    uint16_t speed;
};

struct ACU_TCC_OFFSET_VALUE_REF_DTO
{
    uint16_t axis;
    uint16_t result;
};

struct TCC_ACU_RESET_UPD_DTO
{
    uint16_t target;
};

struct TCC_ACU_ENCODER_RESET_REQ_DTO
{
    uint16_t target;
};

struct TCC_ACU_BRAKE_CTRL_UPD_DTO
{
    uint16_t action;
    uint16_t target;
};

struct TCC_ACU_VALIDATION_CMD_DTO
{
    uint16_t command;
    uint16_t axis;
    uint16_t speed;
};

struct TCC_ACU_PASS_PROJECTILE_CMD_DTO
{
    uint16_t type;
};

struct TCC_ACU_PASS_CHARGE_CMD_DTO
{
    uint16_t type;
};

struct TCC_ACU_SINGLE_DRIVE_CMD_DTO
{
    uint16_t axis;
    uint16_t type;
    uint16_t amount;
    uint16_t action;
    int16_t azimuth;
    int16_t elevation;
    uint16_t speed;
};

struct TCC_ACU_ALIGN_GUN_BREECH_CMD_DTO
{
    uint16_t seq;
    int16_t projectileElevationAngle;
    int16_t chargeElevationAngle;
    uint16_t speed;
};

struct TCC_ACU_LOAD_PROJECTILE_CMD_DTO
{
    uint16_t seq;
    uint16_t projectileElevationAngle;
    uint16_t projectileLoadingAngle;
    uint16_t type;
    uint16_t speed;
};

struct TCC_ACU_LOAD_CHARGE_CMD_DTO
{
    uint16_t seq;
    uint16_t chargeElevationAngle;
    uint16_t chargeLoadingAngle;
    uint16_t type;
    uint16_t speed;
};

struct TCC_ACU_POSITION_DRIVE_CMD_DTO
{
    uint16_t action;
    uint16_t speed;
};

struct TCC_ACU_OFFSET_VALUE_CMD_DTO
{
    uint16_t axis;
    int16_t offset;
};

#pragma pack(pop)