#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t CoreClkHz;
    uint32_t TicksPerSecond;
} systick_config_t;

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

void SYSTICK_Init(void);
void SYSTICK_DelayTicks(uint32_t ticks);
void SYSTICK_InstallCallback(void (*callbackFunc)(void));

void SysTick_Handler(void);
	
#endif





























