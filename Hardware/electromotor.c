#include "electromotor.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

static TIM_HandleTypeDef *const pwm_a[MOTOR_NUM] = {&htim1, &htim1, &htim1, &htim1};
static const uint32_t ch_a[MOTOR_NUM] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};
static TIM_HandleTypeDef *const pwm_b[MOTOR_NUM] = {&htim9, &htim9, &htim10, &htim11};
static const uint32_t ch_b[MOTOR_NUM] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_1, TIM_CHANNEL_1};
static TIM_HandleTypeDef *const enc[MOTOR_NUM] = {&htim2, &htim3, &htim4, &htim8};

void EM_Init(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_NUM; i++)
    {
        HAL_TIM_PWM_Start(pwm_a[i], ch_a[i]);
        HAL_TIM_PWM_Start(pwm_b[i], ch_b[i]);
        HAL_TIM_Encoder_Start(enc[i], TIM_CHANNEL_ALL);
        __HAL_TIM_SET_COMPARE(pwm_a[i], ch_a[i], 0);
        __HAL_TIM_SET_COMPARE(pwm_b[i], ch_b[i], 0);
        __HAL_TIM_SET_COUNTER(enc[i], 0);
    }
}

void EM_SetDuty(uint8_t id, int16_t duty)
{
    int32_t d = duty;
    uint32_t ccr;

    if (id >= MOTOR_NUM)
    {
        return;
    }
    if (d > PWM_DUTY_MAX)
    {
        d = PWM_DUTY_MAX;
    }
    if (d < -PWM_DUTY_MAX)
    {
        d = -PWM_DUTY_MAX;
    }

    if (d > 0)
    {
        ccr = (uint32_t)(d * PWM_PERIOD_A / PWM_DUTY_MAX);
        __HAL_TIM_SET_COMPARE(pwm_a[id], ch_a[id], ccr);
        __HAL_TIM_SET_COMPARE(pwm_b[id], ch_b[id], 0);
    }
    else if (d < 0)
    {
        ccr = (uint32_t)((-d) * PWM_PERIOD_B / PWM_DUTY_MAX);
        __HAL_TIM_SET_COMPARE(pwm_a[id], ch_a[id], 0);
        __HAL_TIM_SET_COMPARE(pwm_b[id], ch_b[id], ccr);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(pwm_a[id], ch_a[id], 0);
        __HAL_TIM_SET_COMPARE(pwm_b[id], ch_b[id], 0);
    }
}

void EM_Stop(void)
{
    uint8_t i;

    for (i = 0; i < MOTOR_NUM; i++)
    {
        EM_SetDuty(i, 0);
    }
}

int16_t EM_GetCount(uint8_t id)
{
    if (id >= MOTOR_NUM)
    {
        return 0;
    }
    return (int16_t)__HAL_TIM_GET_COUNTER(enc[id]);
}

void EM_ResetCount(uint8_t id)
{
    if (id >= MOTOR_NUM)
    {
        return;
    }
    __HAL_TIM_SET_COUNTER(enc[id], 0);
}
