#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 
#include "stepcounter.h"

int main(void)
{ 
//	u8 t=0;			//默认开启上报
//	float pitch,roll,yaw; 		//欧拉角
//	short aacx,aacy,aacz;		//加速度传感器原始数据
//	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//	short temp;					//温度
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(500000);		//初始化串口波特率为500000
//	LCD_Init();					//LCD初始化
//	MPU_Init();					//初始化MPU6050
// 	POINT_COLOR=RED;//设置字体为红色 
	stepcounter_main();
//	POINT_COLOR=BLUE;//设置字体为蓝色 
// 	LCD_ShowString(30,180,200,16,16,"UPLOAD ON ");	 
// 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
// 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
// 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
// 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	 
// 	while(1)
//	{
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 
//			temp=MPU_Get_Temperature();	//得到温度值
//			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//			if((t%10)==0)
//			{ 
//				if(temp<0)
//				{
//					LCD_ShowChar(30+48,200,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,200,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//显示小数部分 
//				temp=pitch*10;
//				if(temp<0)
//				{
//					LCD_ShowChar(30+48,220,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,220,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//显示小数部分 
//				temp=roll*10;
//				if(temp<0)
//				{
//					LCD_ShowChar(30+48,240,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,240,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//显示小数部分 
//				temp=yaw*10;
//				if(temp<0)
//				{
//					LCD_ShowChar(30+48,260,'-',16,0);		//显示负号
//					temp=-temp;		//转为正数
//				}else LCD_ShowChar(30+48,260,' ',16,0);		//去掉负号 
//				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//显示整数部分	    
//				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//显示小数部分  
//				t=0;
//			}
//		}
//		t++; 
//	} 	
}
