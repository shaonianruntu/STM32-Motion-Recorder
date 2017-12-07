#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "systick.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

  
int main(void)
{ 
	u8 t=0;						
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short tmp;					//临时变量
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(500000);		//初始化串口波特率为500000
 	LCD_Init();					//LCD初始化
	MPU_Init();					//初始化MPU6050
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"MPU6050 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/9");
	/* MPU6050 dmp 初始化 */
	while(mpu_dmp_init())
	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		Delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		Delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"MPU6050 OK");
	POINT_COLOR=BLUE;//设置字体为蓝色 
 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	 
 	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			tmp = MPU_Get_Temperature();				//得到已经扩大了100倍的温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			if((t%10)==0)
			{ 
				if(tmp < 0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		//显示负号
					tmp = -tmp;								//转为正数
				}else LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,200,tmp/100,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,200,tmp%10,1,16);		//显示小数部分 
				/* 俯仰角 */
				tmp=pitch*10;								//得到俯仰角,并将其值扩大10倍
				if(tmp < 0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
					tmp = -tmp;		//转为正数
				}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,220,tmp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,220,tmp%10,1,16);		//显示小数部分 
				/* 横滚角 */
				tmp = roll*10;								//得到横滚角，并将其扩大10倍
				if(tmp < 0)	
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
					tmp = -tmp;		//转为正数
				}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,240,tmp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,240,tmp%10,1,16);		//显示小数部分 
				/* 航向角 */
				tmp = yaw*10;
				if(tmp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
					tmp = -tmp;		//转为正数
				}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,260,tmp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,260,tmp%10,1,16);		//显示小数部分  
				
				t=0;
			}
		}
		t++; 
	} 	
}
