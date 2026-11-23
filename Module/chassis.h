#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "main.h"
#include "motor.h"

#define CHASSIS_LX        70.0f
#define CHASSIS_LY        108.0f
#define CHASSIS_V_MAX     1000.0f
#define CHASSIS_W_MAX     5.0f
#define CHASSIS_RAMP      4000.0f

#define CAR_IDLE          0
#define CAR_GOTO          1
#define CAR_PUSH          2
#define CAR_STOP          3

void Chassis_Init(void);
void Chassis_SetSpeed(float vx, float vy, float wz);
void Chassis_Update(float dt);
void Chassis_Control100Hz(void);

extern float chassis_vx;
extern float chassis_vy;
extern float chassis_wz;
extern float chassis_tgt_vx;
extern float chassis_tgt_vy;
extern float chassis_tgt_wz;
extern float pos_x;
extern float pos_y;
extern float yaw;
extern uint8_t car_state;

#endif
