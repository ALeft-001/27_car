#ifndef __BOARD_H
#define __BOARD_H

#include "main.h"

void Board_Init(void);
void Board_Send(const uint8_t *buf, uint16_t len);

extern volatile uint32_t board_last_rx_tick;

#endif
