#include "board.h"
#include "board_Protocol.h"

extern UART_HandleTypeDef huart1;

volatile uint32_t board_last_rx_tick = 0;

static uint8_t board_rx_byte;

void Board_Init(void)
{
    board_last_rx_tick = 0;
    Proto_Init();
    HAL_UART_Receive_IT(&huart1, &board_rx_byte, 1);
}

void Board_Send(const uint8_t *buf, uint16_t len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)buf, len, 10);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        Proto_RxByte(board_rx_byte);
        board_last_rx_tick = HAL_GetTick();
        HAL_UART_Receive_IT(&huart1, &board_rx_byte, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        HAL_UART_Receive_IT(&huart1, &board_rx_byte, 1);
    }
}
