#ifndef __ELECTROMOTOR_H
#define __ELECTROMOTOR_H

#include "main.h"

#define MOTOR_NUM       4
#define PWM_DUTY_MAX    1000
#define PWM_PERIOD_A    8399
#define PWM_PERIOD_B    4199

void EM_Init(void);
void EM_SetDuty(uint8_t id, int16_t duty);
void EM_Stop(void);
int16_t EM_GetCount(uint8_t id);
void EM_ResetCount(uint8_t id);

#endif
