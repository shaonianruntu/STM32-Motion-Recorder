/**
  * @brief 滴答计时器
  */
#include "systick.h"

static __IO u32 TimingCounter;


/**
  * @brief 滴答计时器配置
  */
void mySysTick_Init(void){
		
	if(SysTick_Config(SystemCoreClock/1000000)){	//延时周期为1us
		while(1);	//捕获错误
	}
}

void Delay_us(__IO u32 nTime){
	TimingCounter = nTime;
	while(TimingCounter != 0);
}



void TimingDelay_Decrement(void)
{
	if(TimingCounter != 0x00){
		TimingCounter -= 1;
	}	
}
