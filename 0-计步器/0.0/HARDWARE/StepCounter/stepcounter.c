#include "stepcounter.h"
#include "mpu6050.h"

/**
  *
  */
void StepCounter_Config(){
	Buffer_Init();				//初始化缓冲区
	MPU_Init();					//初始化加速度计
	
}
  
/**
  * @brief 缓冲区初始化
  */
void Buffer_Init(){
	/*清除两个缓冲区全部的数据*/
	for(gBuffer_Grouping = 0; gBuffer_Grouping < ACC_COUNT; gBuffer_Grouping++){
		gBuffer_IsWritable[gBuffer_Grouping] = true;
	}
	gBuffer_Grouping = 0; //当前正在处理第一缓冲区
	gError_Code = ERROR_NULL;	//没有错误
}