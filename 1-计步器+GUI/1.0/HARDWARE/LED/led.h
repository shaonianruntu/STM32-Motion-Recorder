#ifndef __LED_H__
#define __LED_H__


#include"stm32f10x.h"

#define ON  0
#define OFF 1

#define LED1(a) if(a) \
					  GPIO_SetBits(GPIOA,GPIO_Pin_8);\
					  else \
					  GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define Green_LED(b) if(b) \
					  GPIO_SetBits(GPIOD,GPIO_Pin_2);\
					  else \
					  GPIO_ResetBits(GPIOD,GPIO_Pin_2)
					  
void LED_GPIO_Configuration(void);


#endif


