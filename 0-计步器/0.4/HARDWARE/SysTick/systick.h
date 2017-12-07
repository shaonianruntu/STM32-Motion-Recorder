#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void mySysTick_Init(void);
void Delay_us(__IO u32 nTime);

#define Delay_ms(x) 	Delay_us(1000*x)

#endif /* __SYSTICK_H */
