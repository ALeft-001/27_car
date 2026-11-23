#include "chassis.h"
#include "board_Protocol.h"

float chassis_vx;
float chassis_vy;
float chassis_wz;
float chassis_tgt_vx;
float chassis_tgt_vy;
float chassis_tgt_wz;
float pos_x;
float pos_y;
float yaw;
uint8_t car_state;

static float wheel_speed[MOTOR_NUM];
static float wheel_target[MOTOR_NUM];

static void mecanum_inverse(float vx, float vy, float wz, float lx, float ly, float *v)
{
    float k = lx + ly;

    v[0] = vx - vy - k * wz;
    v[1] = vx + vy + k * wz;
    v[2] = vx + vy - k * wz;
    v[3] = vx - vy + k * wz;
}

static void mecanum_forward(const float *v, float lx, float ly, float *vx, float *vy, float *wz)
{
    float k = lx + ly;

    *vx = (v[0] + v[1] + v[2] + v[3]) * 0.25f;
    *vy = (-v[0] + v[1] + v[2] - v[3]) * 0.25f;
    *wz = (-v[0] + v[1] - v[2] + v[3]) / (4.0f * k);
}

static void chassis_ramp(float dt)
{
    float step = CHASSIS_RAMP * dt;

    if (chassis_tgt_vx > chassis_vx + step)
    {
        chassis_vx += step;
    }
    else if (chassis_tgt_vx < chassis_vx - step)
    {
        chassis_vx -= step;
    }
    else
    {
        chassis_vx = chassis_tgt_vx;
    }

    if (chassis_tgt_vy > chassis_vy + step)
    {
        chassis_vy += step;
    }
    else if (chassis_tgt_vy < chassis_vy - step)
    {
        chassis_vy -= step;
    }
    else
    {
        chassis_vy = chassis_tgt_vy;
    }

    if (chassis_tgt_wz > chassis_wz + CHASSIS_W_MAX * dt)
    {
        chassis_wz += CHASSIS_W_MAX * dt;
    }
    else if (chassis_tgt_wz < chassis_wz - CHASSIS_W_MAX * dt)
    {
        chassis_wz -= CHASSIS_W_MAX * dt;
    }
    else
    {
        chassis_wz = chassis_tgt_wz;
    }
}

void Chassis_Init(void)
{
    chassis_vx = 0.0f;
    chassis_vy = 0.0f;
    chassis_wz = 0.0f;
    chassis_tgt_vx = 0.0f;
    chassis_tgt_vy = 0.0f;
    chassis_tgt_wz = 0.0f;
    pos_x = 0.0f;
    pos_y = 0.0f;
    yaw = 0.0f;
    car_state = CAR_IDLE;
}

void Chassis_SetSpeed(float vx, float vy, float wz)
{
    if (vx > CHASSIS_V_MAX)
    {
        vx = CHASSIS_V_MAX;
    }
    if (vx < -CHASSIS_V_MAX)
    {
        vx = -CHASSIS_V_MAX;
    }
    if (vy > CHASSIS_V_MAX)
    {
        vy = CHASSIS_V_MAX;
    }
    if (vy < -CHASSIS_V_MAX)
    {
        vy = -CHASSIS_V_MAX;
    }
    if (wz > CHASSIS_W_MAX)
    {
        wz = CHASSIS_W_MAX;
    }
    if (wz < -CHASSIS_W_MAX)
    {
        wz = -CHASSIS_W_MAX;
    }

    chassis_tgt_vx = vx;
    chassis_tgt_vy = vy;
    chassis_tgt_wz = wz;
}

void Chassis_Update(float dt)
{
    uint8_t i;

    Motor_UpdateSpeed(dt);

    for (i = 0; i < MOTOR_NUM; i++)
    {
        wheel_speed[i] = motor_speed[i] * (float)motor_sign[i];
    }

    mecanum_forward(wheel_speed, CHASSIS_LX, CHASSIS_LY, &chassis_vx, &chassis_vy, &chassis_wz);

    pos_x += chassis_vx * dt;
    pos_y += chassis_vy * dt;
    yaw += chassis_wz * dt;

    chassis_ramp(dt);

    mecanum_inverse(chassis_vx, chassis_vy, chassis_wz, CHASSIS_LX, CHASSIS_LY, wheel_target);

    Motor_SpeedLoop(wheel_target);
}

void Chassis_Control100Hz(void)
{
    if (board_cmd_new)
    {
        board_cmd_new = 0;

        if (board_cmd.cmd == CMD_STOP)
        {
            car_state = CAR_STOP;
            Chassis_SetSpeed(0.0f, 0.0f, 0.0f);
        }
        else if (board_cmd.cmd == CMD_GOTO)
        {
            car_state = CAR_GOTO;
        }
        else if (board_cmd.cmd == CMD_PUSH)
        {
            car_state = CAR_PUSH;
        }
    }

    if ((HAL_GetTick() - board_cmd_tick) > 200)
    {
        car_state = CAR_STOP;
    }

    if (car_state == CAR_STOP)
    {
        Chassis_SetSpeed(0.0f, 0.0f, 0.0f);
    }
}
