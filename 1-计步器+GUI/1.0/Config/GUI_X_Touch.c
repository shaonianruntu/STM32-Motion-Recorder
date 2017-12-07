/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"

#include "touch.h"


/*启动触摸屏X轴的测量，实际可以不用改动这个函数*/
void GUI_TOUCH_X_ActivateX(void) {
}
/*启动触摸屏Y轴的测量，实际可以不用改动这个函数*/
void GUI_TOUCH_X_ActivateY(void) {
}

/*返回A/D转换后X轴的位置*/
int  GUI_TOUCH_X_MeasureX(void) 
{
	u16 x,y;
	Read_ADS2(&x,&y);

	return x;
}
/*返回A/D转换后Y轴的位置*/
int  GUI_TOUCH_X_MeasureY(void) 
{
	u16 x,y;
	Read_ADS2(&x,&y);

	return y;
}



