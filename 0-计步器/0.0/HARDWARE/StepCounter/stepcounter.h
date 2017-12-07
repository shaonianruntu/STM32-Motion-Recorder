#ifndef __STEPCOUNTER_H
#define __STEPCOUNTER_H

#include "mpu6050.h"
#include <stdbool.h>

/*采样缓冲区设置*/
#define BUFFER_GROUP_COUNT 			2			//设置2个缓冲区
#define POINT_COUNT					100			//采样点总数，2*50	
#define POINT_PER_GROUP				(POINT_COUNT/BUFFER_GROUP_COUNT)	//每个缓冲区的采样点数目
uint16_t gBuffer_Grouping;						//当前正在处理的缓冲区区号
bool gBuffer_IsWritable[BUFFER_GROUP_COUNT];	//该缓冲区是否可写

#define ACC_COUNT					3			//加速度值纬度

/*数据异常信号*/
#define ERROR_NULL					0			//运行正常，不存在错误
#define ERROR_OVERFLOW				1			//缓冲区数据采样溢出
uint8_t gError_Code					0			//错误标号

/*波形数据范围*/
#define DETECTOR_MIN_PPValue		3000		//波形的最小峰峰值
#define DETECTOR_MAX_Value 			32767		//波值的最大上限
#define DETECTOR_MIN_Value 			(-32768)	//波值的最大下限







/*记录三个纬度的数据*/
int16_t gDetector_Max_Value[ACC_COUNT];			//获取三个纬度的波峰值
int16_t gDetector_Min_Value[ACC_COUNT];			//获取三个纬度的波谷值
int16_t gDetector_Threshold[ACC_COUNT];			//保存三个纬度的判断阈值
int16_t gDetector_Threshold_Num	= ACC_COUNT;	//阈值索引编号




#endif