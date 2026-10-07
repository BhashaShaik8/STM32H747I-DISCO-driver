#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdint.h>

void     USART_Init(uint32_t baud);
void     USART_Write_Char(char c);
void     USART_Write_String(const char *s);
uint32_t USART_Rx_Ready(void);      /* 1 = a byte is waiting */
char     USART_Read_Char(void);     /* waits until a byte arrives */

#endif