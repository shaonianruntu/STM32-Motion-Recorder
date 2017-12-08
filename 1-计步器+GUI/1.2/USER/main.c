#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "GUI.h"
#include "touch.h"
#include "timer.h"
  
 
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	TIM2_Int_Init(100-1,8400-1);	//中断10ms
	tp_dev.init();				//触摸屏初始化

	GUI_Init();	

	
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	
	GUI_CURSOR_Show();	//显示指针
	Task_One();
	
	
    while(1){
		GUI_Exec();	//功能运行函数，比如更新函数
	}
}	

