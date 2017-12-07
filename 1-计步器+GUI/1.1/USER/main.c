#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "GUI.h"
#include "touch.h"
#include "timer.h"
#include "stepcounter.h"
 
extern void JBQ_Draw_Time(void);
extern void JBQ_Draw_TopThree(void);
extern void JBQ_Draw_Panel(void);
extern void ZNZ_Draw_Compass();
extern void GPS_Draw_Satellite();

int main(void)
{
//	int j=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
//	TIM2_Int_Init(100-1,8400-1);	//中断10ms

//	stepcounter_main();
	GUI_Init();		//LCD初始化
	GUI_SetBkColor(0x282C30);
	GUI_Clear();
	tp_dev.init();				//触摸屏初始化
	
	ZNZ_Draw_Compass();
	GPS_Draw_Satellite();
	
	JBQ_Draw_Time();
	JBQ_Draw_TopThree();
	JBQ_Draw_Panel();

	stepcounter_main();

	
//    while(1){
//	//	GUI_DispDecAt(j++,600,400,5);
//	//	if(j>=99999)j=0;
//		GUI_Exec();	//功能运行函数，比如更新函数
//	}
}	

