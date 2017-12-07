#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "touch.h" 
#include "GUI.h"
#include "timer.h"
/******************************************************************/
//哈喽，大家好，你现在收看的是三文鱼为你讲解的ucGUI教学视频！！！//
//我还是秉持着 简单、高效、实战的风格给大家提供快速应用的方法	 //
//此教程不是传统的说教！！！不会讲的高深莫测！！！				 //
//我崇尚：快速使用、简单入手、节约时间！！！					 //
//日期：2013/12/27   
/******************************************************************/

int main(void)
{			 
  	Stm32_Clock_Init(9);
	delay_init(72);		
	uart_init(72,9600); 
	Timerx_Init(100,7199); //中断 10ms
	Touch_Init();//触摸屏初始化
		
	GUI_Init();
    GUI_SetBkColor(GUI_BLUE); 
	GUI_Clear();

	Task_1();


 	while(1)
	{
		GUI_Exec();//功能运行函数，比如更新窗口
	}

											    
}


