#include "motor.h"
#include "electromotor.h"

float motor_speed[MOTOR_NUM];
int16_t motor_duty[MOTOR_NUM];
int16_t motor_sign[MOTOR_NUM];
pid_ctrl_t motor_pid[MOTOR_NUM];

static int16_t last_cnt[MOTOR_NUM];

void Motor_Init(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_NUM; i++)
    {
        motor_pid[i].kp = 2.0f;
        motor_pid[i].ki = 0.05f;
        motor_pid[i].kd = 0.0f;
        motor_pid[i].err = 0.0f;
        motor_pid[i].last_err = 0.0f;
        motor_pid[i].integral = 0.0f;
        motor_pid[i].integral_max = 400.0f;
        motor_pid[i].pout = 0.0f;
        motor_pid[i].iout = 0.0f;
        motor_pid[i].dout = 0.0f;
        motor_pid[i].last_dout = 0.0f;
        motor_pid[i].out = 0.0f;
        motor_pid[i].out_max = (float)PWM_DUTY_MAX;

        motor_speed[i] = 0.0f;
        motor_duty[i] = 0;
        motor_sign[i] = 1;
        last_cnt[i] = 0;
    }
}

void Motor_UpdateSpeed(float dt)
{
    int16_t c;
    uint8_t i;

    if (dt <= 0.0f)
    {
        return;
    }

    for (i = 0; i < MOTOR_NUM; i++)
    {
        c = EM_GetCount(i);
        motor_speed[i] = (float)(int16_t)(c - last_cnt[i]) * CNT2MM / dt;
        last_cnt[i] = c;
    }
}

void Motor_SpeedLoop(const float *target)
{
    uint8_t i;
    int16_t out;

    for (i = 0; i < MOTOR_NUM; i++)
    {
        motor_pid[i].err = target[i] - motor_speed[i];
        single_pid_ctrl(&motor_pid[i]);

        out = (int16_t)motor_pid[i].out;
        if (out > PWM_DUTY_MAX)
        {
            out = PWM_DUTY_MAX;
        }
        if (out < -PWM_DUTY_MAX)
        {
            out = -PWM_DUTY_MAX;
        }
        motor_duty[i] = (int16_t)(out * motor_sign[i]);
        EM_SetDuty(i, motor_duty[i]);
    }
}

void Motor_Stop(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_NUM; i++)
    {
        motor_pid[i].integral = 0.0f;
        motor_pid[i].out = 0.0f;
        motor_duty[i] = 0;
    }
    EM_Stop();
}
