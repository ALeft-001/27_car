#include "main.h"
#include "cmsis_os2.h"
#include "board.h"
#include "board_Protocol.h"
#include "chassis.h"

void Task_Board(void)
{
    uint8_t buf[PROTO_ST_LEN];

    for (;;)
    {
        Proto_PackState(buf, car_state, (int16_t)pos_x, (int16_t)pos_y, (int16_t)(yaw * 57.29578f));
        Board_Send(buf, PROTO_ST_LEN);
        osDelay(100);
    }
}
