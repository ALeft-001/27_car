#include "main.h"
#include "cmsis_os2.h"
#include "chassis.h"

void Task_Control(void)
{
    uint32_t tick = osKernelGetTickCount();
    uint8_t div = 0;

    for (;;)
    {
        Chassis_Update(0.001f);

        div++;
        if (div >= 10)
        {
            div = 0;
            Chassis_Control100Hz();
        }

        tick += 1;
        osDelayUntil(tick);
    }
}
