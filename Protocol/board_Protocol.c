#include "board_Protocol.h"

BoardCmd_t board_cmd;
volatile uint8_t board_cmd_new = 0;
volatile uint32_t board_cmd_tick = 0;

static uint8_t rx_buf[PROTO_CMD_LEN];
static uint8_t rx_idx = 0;
static uint8_t rx_sum = 0;

static uint8_t proto_sum(const uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t s = 0;

    for (i = 0; i < len; i++)
    {
        s ^= buf[i];
    }
    return s;
}

void Proto_Init(void)
{
    rx_idx = 0;
    rx_sum = 0;
    board_cmd.cmd = CMD_STOP;
    board_cmd.x = 0;
    board_cmd.y = 0;
    board_cmd.th = 0;
    board_cmd_new = 0;
}

void Proto_RxByte(uint8_t b)
{
    if (rx_idx == 0 && b != PROTO_HEAD_CMD)
    {
        return;
    }

    rx_buf[rx_idx] = b;

    if (rx_idx < PROTO_CMD_LEN - 1)
    {
        rx_sum ^= b;
    }

    rx_idx++;

    if (rx_idx >= PROTO_CMD_LEN)
    {
        if (proto_sum(rx_buf, PROTO_CMD_LEN - 1) == rx_buf[PROTO_CMD_LEN - 1])
        {
            board_cmd.cmd = rx_buf[1];
            board_cmd.x = (int16_t)(((uint16_t)rx_buf[2] << 8) | rx_buf[3]);
            board_cmd.y = (int16_t)(((uint16_t)rx_buf[4] << 8) | rx_buf[5]);
            board_cmd.th = (int16_t)(((uint16_t)rx_buf[6] << 8) | rx_buf[7]);
            board_cmd_new = 1;
            board_cmd_tick = HAL_GetTick();
        }
        rx_idx = 0;
        rx_sum = 0;
    }
}

uint8_t Proto_PackState(uint8_t *buf, uint8_t state, int16_t x, int16_t y, int16_t yaw)
{
    uint8_t i;

    buf[0] = PROTO_HEAD_STATE;
    buf[1] = state;
    buf[2] = (uint8_t)((uint16_t)x >> 8);
    buf[3] = (uint8_t)((uint16_t)x & 0xFF);
    buf[4] = (uint8_t)((uint16_t)y >> 8);
    buf[5] = (uint8_t)((uint16_t)y & 0xFF);
    buf[6] = (uint8_t)((uint16_t)yaw >> 8);
    buf[7] = (uint8_t)((uint16_t)yaw & 0xFF);

    for (i = 0; i < PROTO_ST_LEN - 1; i++)
    {
        buf[PROTO_ST_LEN - 1] = 0;
    }
    buf[PROTO_ST_LEN - 1] = proto_sum(buf, PROTO_ST_LEN - 1);
    return PROTO_ST_LEN;
}
