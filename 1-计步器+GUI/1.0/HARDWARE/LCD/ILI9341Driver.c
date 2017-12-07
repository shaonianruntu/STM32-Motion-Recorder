#include "ILI9341Driver.h"
#include "stdlib.h"
#include "SysTick.h"	 

//画笔颜色,背景颜色
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 					    
//写寄存器函数
//data:寄存器值
void LCD_WR_REG(u8 data)
{ 
	LCD_RS_CLR;//写地址  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
	   
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{										   
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0X0000;     //全部输出0
	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读寄存器时,并不需要读2次)
	LCD_RD_CLR;
	if(DeviceCode==0X8989)delay_us(2);//FOR 8989,延时2us					   
	LCD_RD_SET;
	t=DATAIN;  
	LCD_CS_SET; 
 
	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}		    

//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}
	   
//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
	return LCD_RD_DATA(); 
}  
 
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	if(DeviceCode==0X9341)LCD_WR_REG(0x2C);//ILI9341是以0X2C开始的
	else LCD_WR_REG(R34);
}	
 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//读取个某点的颜色值	 
//x:0~239
//y:0~319
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	if(x>=LCD_Width||y>=LCD_High)return 0;			//超过了范围,直接返回		   
	LCD_SetCursor(x,y);
	if(DeviceCode==0X9341)LCD_WR_REG(0X2E);	//ILI9341发送读GRAM指令
	else LCD_WR_REG(R34);      		 		//其他IC发送读GRAM指令
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高

	LCD_RS_SET;
	LCD_CS_CLR;
	//读取数据(读GRAM时,第一次为假读)	
	LCD_RD_CLR;	
  	delay_us(1);//延时1us					   
	LCD_RD_SET;
 	//dummy READ
	LCD_RD_CLR;					   
	delay_us(1);//延时1us					   
	LCD_RD_SET;
 	r=DATAIN;  
 	if(DeviceCode==0X9341)
	{	 
		LCD_RD_CLR;					   
	 	LCD_RD_SET;
		b=DATAIN;//读取蓝色值  	  
		g=r&0XFF;//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	}	 
	LCD_CS_SET;

	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高  
	if(DeviceCode==0X9325||DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return r;	//这几种IC直接返回颜色值
	else if(DeviceCode==0X9341)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));						//ILI9341需要公式转换一下
	else return LCD_BGR2RGB(r);																	//其他IC
}
//LCD开启显示
void LCD_DisplayOn(void)
{	
	if(DeviceCode==0X9341)LCD_WR_REG(0X29);	//开启显示
	else LCD_WriteReg(R7,0x0173); 			//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	if(DeviceCode==0X9341)LCD_WR_REG(0X28);	//关闭显示
	else LCD_WriteReg(R7, 0x0);//关闭显示 
}   
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
__inline void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
#if USE_HORIZONTAL==1 
	if(DeviceCode==0X8989)
	{
		Xpos=319-Xpos;
		LCD_WriteReg(0X4E, Ypos);
		LCD_WriteReg(0X4F, Xpos);
    }else if(DeviceCode==0X9341)//9341,设置坐标
	{			 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);			 
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);
	}else
	{
		Xpos=319-Xpos;
		LCD_WriteReg(R32,Ypos);
		LCD_WriteReg(R33,Xpos);
	}							   
#else
	if(DeviceCode==0X8989)
	{
		LCD_WriteReg(0X4E, Xpos);
		LCD_WriteReg(0X4F, Ypos);
    }else if(DeviceCode==0X9341)
	{		    
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);
	}else
	{
		LCD_WriteReg(R32, Xpos);
		LCD_WriteReg(R33, Ypos);
	}						    
#endif
}
//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341这个奇葩),所以,
//一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//0~7：代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341等IC已经实际测试	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
#if USE_HORIZONTAL//使用横屏
	switch(dir)//方向转换
	{
		case 0:dir=6;break;
		case 1:dir=7;break;
		case 2:dir=4;break;
		case 3:dir=5;break;
		case 4:dir=1;break;
		case 5:dir=0;break;
		case 6:dir=3;break;
		case 7:dir=2;break;	     
	}
#endif
	if(DeviceCode==0x9341)//9341,很特殊
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		LCD_WriteReg(dirreg,regval);
		if(regval&0X20)
		{
 	 		LCD_WR_REG(0x2A); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA(319>>8);LCD_WR_DATA(319&0XFF);
	 		LCD_WR_REG(0x2B); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA(239>>8);LCD_WR_DATA(239&0XFF);
		}else 
		{
 	 		LCD_WR_REG(0x2A); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA(239>>8);LCD_WR_DATA(239&0XFF);
	 		LCD_WR_REG(0x2B); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA(319>>8);LCD_WR_DATA(319&0XFF);
		}  
 	}else 
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(DeviceCode==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//其他驱动IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
}  
//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}  
void LCD_SetPoint(u16 x,u16 y,u16 color)
{
	LCD_SetCursor(x,y);//设置光标位置 
//	LCD_WR_REG(R34);//开始写入GRAM
	LCD_WriteRAM_Prepare();
	LCD_WR_DATA(color); 
} 	  
//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void TFT_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
/*STM32 MCU上电复位之后默认PA13/14/15；PB3/4为JTAG功能，*/
/*所以如果配置为普通IO，要改变指定管脚的映射*/	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//JTAG对应的IO失能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//数据端口 PB IO  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
 
	delay_ms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	DeviceCode = LCD_ReadReg(0x0000);    
 	if(DeviceCode==0||DeviceCode==0XFFFF)//读到ID不正确
	{	
		//可能是9341,尝试9341的ID读取		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 			//dummy read 	
 		LCD_RD_DATA();   	    //读回0X00
  		DeviceCode=LCD_RD_DATA()&0XBF;//这里读回0XD3,实际是0X93才对.强制去掉第6位   								   
 		DeviceCode<<=8;
		DeviceCode|=LCD_RD_DATA();	     
	}
//	printf(" LCD ID:%x\r\n",DeviceCode); //打印LCD ID 
	if(DeviceCode==0X9341)	//9341初始化
	{
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
	}
	LCD_Scan_Dir(DFT_SCAN_DIR);		 
	LCD_LED;//点亮背光	 

}  		 
//清屏函数
//Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 

//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	LCD_Scan_Dir(L2R_U2D);
#if USE_HORIZONTAL==1
	xlen=ey-sy+1;	   
	for(i=sx;i<=ex;i++)
	{
	 	LCD_SetCursor(i,sy);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	  
	}
#else
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
#endif
	LCD_Scan_Dir(DFT_SCAN_DIR);
}  

