#include"led.h"


void LED_GPIO_Configuration(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	/*使能GPIOA的外设时钟*/
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA,ENABLE);
	/*选择控制的GPIOA的引脚*/
 	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	/*设置引脚为推挽模式输出*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	/*配置引脚输出频率*/
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	/*调用库，初始化GPIOA*/
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*设置GPIOA的某一位输出高电平*/
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   //使能PD端口时钟
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
	 GPIO_SetBits(GPIOD,GPIO_Pin_2); 


}



