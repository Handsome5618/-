#ifndef __EACH_H
#define	__EACH_H

#include "stm32f10x.h"
#define SET 1000
void Usart_Receive_Data(int16_t data);
void Usart_Data(void);
typedef  struct
{
    int distance_1;
    int distance_2;
    int angle;
    int stata;
    int add_x;
} Usart_data;

Usart_data* get_Usart1_data_point(void);


#endif
