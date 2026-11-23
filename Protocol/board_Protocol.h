#ifndef __BOARD_PROTOCOL_H
#define __BOARD_PROTOCOL_H

#include "main.h"

#define PROTO_HEAD_CMD   0xAA
#define PROTO_HEAD_STATE 0x55
#define PROTO_CMD_LEN    9
#define PROTO_ST_LEN     9

#define CMD_STOP         0
#define CMD_GOTO         1
#define CMD_PUSH         2

typedef struct
{
    uint8_t cmd;
    int16_t x;
    int16_t y;
    int16_t th;
} BoardCmd_t;

extern BoardCmd_t board_cmd;
extern volatile uint8_t board_cmd_new;
extern volatile uint32_t board_cmd_tick;

void Proto_Init(void);
void Proto_RxByte(uint8_t b);
uint8_t Proto_PackState(uint8_t *buf, uint8_t state, int16_t x, int16_t y, int16_t yaw);

#endif
