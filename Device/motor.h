#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "pid.h"
#include "electromotor.h"

#define MOTOR_REDUCTION 30
#define ENCODER_PPR     11
#define ENCODER_CPR     (ENCODER_PPR * MOTOR_REDUCTION * 4)
#define WHEEL_R         30.0f
#define CNT2MM          (6.2831853f * WHEEL_R / (float)ENCODER_CPR)

void Motor_Init(void);
void Motor_UpdateSpeed(float dt);
void Motor_SpeedLoop(const float *target);
void Motor_Stop(void);

extern float motor_speed[MOTOR_NUM];
extern int16_t motor_duty[MOTOR_NUM];
extern int16_t motor_sign[MOTOR_NUM];
extern pid_ctrl_t motor_pid[MOTOR_NUM];

#endif
