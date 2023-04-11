#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>
extern u8 com_data;

extern int sign_usart;
void PC_SendString(uint8_t *str);
void USART3_Config(void);
void UART3Test(void);
#endif /* __USART1_H */
