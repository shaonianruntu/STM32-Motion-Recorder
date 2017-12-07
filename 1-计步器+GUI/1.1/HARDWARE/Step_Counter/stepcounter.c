#include "stepcounter.h"
#include "stdio.h"
extern void JBQ_Draw_Panel(void);

static uint32_t gTickCounterForMPU6050 = 0;	
//volatile bool bFlagForMPU6050 = false;

#define SAMPLE_POINT_COUNT			100 		//缓冲区采样点总数 2*50
#define SAMPLE_BUFFER_GROUP_COUNT	2			//将一个缓冲区分为2个缓冲段
#define SAMPLE_POINT_PER_GROUP 		(SAMPLE_POINT_COUNT/SAMPLE_BUFFER_GROUP_COUNT)	//每个缓冲段的采样点
#define SAMPLE_AVERAGE_COUNT		4
#define SAMPLE_ACC_COUNT            3			//加速度计采样纬度数
#define SAMPLE_POINT_IN_TICKS       4			//

#define ERROR_EVERYTHING_IS_OK      0
#define ERROR_SAMPLE_OVERFLOW       1

#define DETECTOR_MIN_PPValue 		(3000)		//峰峰值差分的最小值

#define DETECTOR_MAX_Value  (32767)				//检波器最大值,16位数
#define DETECTOR_MIN_Value  (-32768)			//检波器最小值，16位数

uint8_t gErrorCode = 0;

/* 采样数据 */
int16_t gSampleAccValue[SAMPLE_ACC_COUNT][SAMPLE_POINT_COUNT];	//缓冲区三个纬度上的峰峰值
uint32_t gSampleIndex = 0;						//当前正在操作的时间点点好
uint32_t gSampleGroupIndex = 0;					//当前正在操作的缓冲段段号
bool gSampleGroupWritable[SAMPLE_BUFFER_GROUP_COUNT];	//缓冲段可写性记录

/* 峰峰值 */
int16_t gSampleValueMax[SAMPLE_ACC_COUNT];
int16_t gSampleValueMin[SAMPLE_ACC_COUNT];

/* 缓冲区检测阈值点 */
int16_t gSampleShresholdValueForDetector[SAMPLE_ACC_COUNT];
uint32_t gSampleShresholdIdxForDetector = SAMPLE_ACC_COUNT;	//上次峰峰值所在的纬度

/* This value would be the corssing point and comparied with every sample point . */
int16_t gSamplePrePPValue;
uint32_t gSamplePrePPIndex;
uint32_t gSampleMidPPIndex;

/* 均值滤波 */
volatile uint32_t gFilterIndex[SAMPLE_ACC_COUNT] = {0};
volatile int16_t gFilterBuffer[SAMPLE_ACC_COUNT][SAMPLE_AVERAGE_COUNT] = {0};

/* main进程空闲时间计算 */
volatile uint32_t gCurTicker;

/* 计步数量累计 */
volatile uint32_t gStepCounter = 0; 

static void TickCallback(void);

int16_t sample_average_filter(uint32_t group, int16_t val);




/*********************************************/

int16_t aacx=0,aacy=0,aacz=0;		//加速度传感器原始数据

int16_t max_diff;
int32_t sample_diff;


int32_t dfnn = 0;
/************************************************/


void stepcounter_main(void){
	uint32_t i,j;
	char ac[6];
	
	/* 板载资源初始化 */
	MPU_Init();		//初始化MPU6050	
	LED_Init();		//LED初始化
	TIM3_Int_Init(50-1,8400-1);	//定时器时钟84M，分频系数为84M/8400=10kHz,计数50次，即5ms
	while(mpu_dmp_init())
	{
//		LCD_ShowString(30,30,200,16,16,"MPU6050 Error");
//		delay_ms(200);
//		LCD_Fill(30,30,320,480,WHITE);
// 		delay_ms(200);
	}
//	/* TFT显示提示信息 */
//	LCD_ShowString(30,30,200,16,16,"************************");	
//	LCD_ShowString(30,50,200,16,16,"Hello, Stepter.");
//	LCD_ShowString(30,70,200,16,16,"************************");	
	
	/* 初始化进程 */
	for(gSampleGroupIndex = 0; gSampleGroupIndex < SAMPLE_BUFFER_GROUP_COUNT; gSampleGroupIndex++){
		gSampleGroupWritable[gSampleGroupIndex] = true;			//标记数据可写入，即清除原来的数据
	}
	gSampleGroupIndex = 0;
	gErrorCode = ERROR_EVERYTHING_IS_OK;
	
	TIM3_InstallCallback(TickCallback);
	gCurTicker = 0xFFFFFFFF;
	
	/* 开始监视 */
	while(1){
		max_diff = 0;
		sample_diff = 0;
		/* 错误检测 */
        if (ERROR_EVERYTHING_IS_OK != gErrorCode)
        {
//          LCD_ShowString(30,90,200,16,16,"Error #");
//			LCD_ShowNum(30+72,90,gErrorCode,1,16);
            break;
        }
//		/*显示最新的计步值*/
//		LCD_ShowString(30,90,200,16,16,"Steper:");
//		LCD_ShowNum(30+7*8,90,gStepCounter,5,16);
		
		/************************************************************************/
		/*                       实现响应小循环，结束                            */
		/************************************************************************/
		/* 缓冲区数据为空，跳过数据处理 */
		if(gSampleGroupWritable[gSampleGroupIndex]){	
			continue;
		}	

		/* 对缓冲区中的数据进行处理 */
		/*
		1.扫描缓冲区的每个采样点的三个纬度，进行均值滤波。
		2.在上一个缓冲区中得到的峰峰值最大的纬度进行比较，得到计步数量的累加。
		3.进行本缓冲区三个采样点峰峰值的计算，得到最大的峰峰值及其纬度。
		4.将本缓冲区最大峰峰值的纬度同上一个缓冲区进行比较，若一致，则交换，否则，滤过。
		*/
		
		/* 程序设计的进程：
        1. 对采样数据进行均值滤波。
        2. 对采样数据进行峰值检测，更新下一周期判断步伐的检测阈值，包括滤波。
        3. 检测步伐，进行模式匹配。
        */

        /* 空闲时间检测，作为main函数处理完数据之后的空余时间
        这个值越大，表示在采样在下一个缓冲区填充完毕之前还可以插入更多的处理过程
        在理想情况下，用户数据处理使用0个滴答，那么空闲时间就应该是完成一个缓冲区采样之间的时间。
        但实际上，处理数据会用掉一点时间，那么空闲时间就应该是缓冲区采样时间间隔减去处理数据用掉的时间。
        */
		gCurTicker = 0xFFFFFFFF;
		
		/* 初始化峰峰值检测 */
		for(j=0;j<SAMPLE_ACC_COUNT;j++){
			gSampleValueMax[j] = DETECTOR_MIN_Value;
			gSampleValueMin[j] = DETECTOR_MAX_Value;
		}
		
		/************************************************************************/
		/*                       消化一个缓冲区的采样点                           */
		/************************************************************************/
		for(i = gSampleGroupIndex*SAMPLE_POINT_PER_GROUP; i < ((gSampleGroupIndex + 1)*SAMPLE_POINT_PER_GROUP); i++){	//当前处理的缓冲段
			/* 分别处理3个采样轴的采样点 */
			for(j = 0; j < SAMPLE_ACC_COUNT; j++){
				/* 扫描缓冲区的每个采样点的三个纬度，进行均值滤波 */
				gSampleAccValue[j][i] = sample_average_filter(j, gSampleAccValue[j][i]);
				
				/* 在上一个缓冲区中得到的峰峰值最大的纬度进行比较，得到计步数量的累加 */
				if(gSampleShresholdIdxForDetector == j){
					if((gSampleAccValue[j][i] >= gSampleShresholdValueForDetector[j]) 
					   && (gSampleAccValue[j][i-1] < gSampleShresholdValueForDetector[j])){
							/* 越过阈值，计步数量加1 */
							gStepCounter++;
						}
				}
				
				/* 更新最大和最小值 */
				if(gSampleAccValue[j][i] > gSampleValueMax[j]){
					gSampleValueMax[j] = gSampleAccValue[j][i];
				}
				if(gSampleAccValue[j][i] < gSampleValueMin[j]){
					gSampleValueMin[j] = gSampleAccValue[j][i];
				}
			}
		}
		
		/* 清空已经处理过的缓冲区，转到下一个缓冲区 */
		gSampleGroupWritable[gSampleGroupIndex] = true;
		gSampleGroupIndex++;
		if(gSampleGroupIndex >= SAMPLE_BUFFER_GROUP_COUNT){
			gSampleGroupIndex = 0;
		}
		
		/************************************************************************/
		/*                	     		更新判断阈值	     	                    */
		/************************************************************************/
		/* 
		1.得到三个轴上的判决阈值，gSampleShresholdValueForDetector[ii]
		2.得到监控轴，gSampleShresholdIdxForDetector
		*/
		gSampleShresholdIdxForDetector = SAMPLE_ACC_COUNT; //监控轴自动更新
		max_diff = DETECTOR_MIN_Value;	
		for(j = 0; j < SAMPLE_ACC_COUNT; j++){
			sample_diff = gSampleValueMax[j] - gSampleValueMin[j];	//差分值
			
			if(sample_diff >= DETECTOR_MIN_PPValue){
				gSampleShresholdValueForDetector[j] = gSampleValueMax[j] - (int16_t)(sample_diff / 4);	//阈值为峰峰值的60%
				if(gSampleShresholdValueForDetector[j] > max_diff){
					max_diff = gSampleShresholdValueForDetector[j];
					gSampleShresholdIdxForDetector = j;
				}
			}
			else{
				gSampleShresholdValueForDetector[j] = DETECTOR_MAX_Value;
			}
		}
		
		/************************************************************************/
		/*                	     		显示中间值 	     	                    */
		/************************************************************************/
//		LCD_ShowNum(30,120,gSampleValueMin[0],5,16);	
//		LCD_ShowNum(30+5*8+8,120,gSampleValueMax[0],5,16);	
//		LCD_ShowNum(30+10*8+2*8,120,gSampleValueMax[0]-gSampleValueMin[0],5,16);
//		
//		LCD_ShowNum(30,140,gSampleValueMin[1],5,16);	
//		LCD_ShowNum(30+5*8+8,140,gSampleValueMax[1],5,16);	
//		LCD_ShowNum(30+10*8+2*8,140,gSampleValueMax[1]-gSampleValueMin[1],5,16);
//		
//		LCD_ShowNum(30,160,gSampleValueMin[2],5,16);	
//		LCD_ShowNum(30+5*8+8,160,gSampleValueMax[2],5,16);	
//		LCD_ShowNum(30+10*8+2*8,160,gSampleValueMax[2]-gSampleValueMin[2],5,16);
//		
////		LCD_ShowString(30,180,200,16,16,"DetectorVal:");
//		LCD_ShowNum(30,180,gSampleShresholdValueForDetector[0],5,16);
//		LCD_ShowNum(30+6*8,180,gSampleShresholdValueForDetector[1],5,16);
//		LCD_ShowNum(30+12*8,180,gSampleShresholdValueForDetector[2],5,16);
//		
//		LCD_ShowString(30,200,200,16,16,"DetectorIdx:");
//		LCD_ShowNum(30+12*8,200,gSampleShresholdIdxForDetector,5,16);
//		
//		LCD_ShowString(30,220,200,16,16,"************************");	
//		LCD_ShowString(30,240,200,16,16,"StepCounter: ");	
//		LCD_ShowNum(30+14*8,240,gStepCounter,5,16);
//		LCD_ShowString(30,260,200,16,16,"************************");	
//		
//		LCD_ShowString(30,280,200,16,16,"Time remain:");
//		LCD_ShowNum(30+12*8,280,(SAMPLE_POINT_PER_GROUP*SAMPLE_POINT_IN_TICKS) - (0xFFFFFFFF-gCurTicker),5,16);	


		GUI_SetColor(0x12b9ff);
		GUI_SetFont(&GUI_FontD24x32);
		sprintf(ac,"%d",gStepCounter);
		GUI_DispStringHCenterAt(ac,120,160);
	}	
}
	
static void TickCallback(void){
	aacx=0,aacy=0,aacz=0;
	
    if (gCurTicker > 0){
        gCurTicker--;
    }
	if(gTickCounterForMPU6050 >= SAMPLE_POINT_IN_TICKS){ 		/* 50Hz */
		gTickCounterForMPU6050 = 0;
		/* 得到加速度传感器数据 */
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);		
		/* 缓冲区位满复位 */
		if(gSampleIndex >= SAMPLE_POINT_COUNT){
            gSampleIndex = 0;
        }
		/* 填充一个缓冲区 */
		if(gSampleGroupWritable[gSampleIndex/SAMPLE_POINT_PER_GROUP]){
			gSampleAccValue[0][gSampleIndex] = aacx;
			gSampleAccValue[1][gSampleIndex] = aacy;
			gSampleAccValue[2][gSampleIndex] = aacz;
			/* 当前这个点是该缓冲区的最后一个点，填充之后关闭写使能，使main进程可以读到。 */
            if (0 == (gSampleIndex+1)%SAMPLE_POINT_PER_GROUP ) 
            {
                gSampleGroupWritable[gSampleIndex / SAMPLE_POINT_PER_GROUP] = false; /* lock the buffer. */
            }
            gSampleIndex++;
		}
		else{
            //gErrorCode = ERROR_SAMPLE_OVERFLOW;
        }	
	}
	else{
        gTickCounterForMPU6050++;
    }
}

/* 均值滤波 */
int16_t sample_average_filter(uint32_t group, int16_t val){
	int32_t sum;
	uint32_t i;
	
	/* 缓冲区数据更新 */
    if (gFilterIndex[group] >= SAMPLE_AVERAGE_COUNT){
        gFilterIndex[group] = 0;
    }
	gFilterBuffer[group][gFilterIndex[group]] = val;	//重复记录4次这个缓存区该点的三纬数据
    gFilterIndex[group]++;
	
	/* 数值累加 */
	sum = 0;
	for(i = 0; i < SAMPLE_AVERAGE_COUNT; i++){
		sum += gFilterBuffer[group][i];
	}
	return sum/SAMPLE_AVERAGE_COUNT;
}
