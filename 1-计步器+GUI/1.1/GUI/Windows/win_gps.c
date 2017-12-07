#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"

#include "math.h"
#include "stdio.h"

extern GUI_FONT GUI_FontHZ_NewSimSun_11;
extern GUI_FONT GUI_FontHZ_NewSimSun_12;
/**
  * @brief 卫星状态显示
  */
void GPS_Draw_Satellite(){

	char ac[5]; 

	GUI_SetColor(0x8e8e90);
	GUI_SetFont(&GUI_FontHZ_NewSimSun_12);
	GUI_SetTextAlign(GUI_TA_LEFT);
	GUI_DispStringAt("当前卫星数",0,280);	

	GUI_SetColor(0x8e8e90);
	GUI_SetFont(&GUI_FontHZ_NewSimSun_12);
	GUI_SetTextAlign(GUI_TA_LEFT);
	GUI_DispStringAt("可见卫星数",0,295);

	GUI_SetColor(0x8e8e90);
	GUI_SetFont(&GUI_FontHZ_NewSimSun_12);
	GUI_SetTextAlign(GUI_TA_LEFT);
	GUI_DispStringHCenterAt("海拔",200,280);

	GUI_SetColor(0x8e8e90);
	GUI_SetFont(&GUI_Font13_ASCII);
	sprintf(ac,"%d",4);
	GUI_DispStringHCenterAt(ac,72,279);

	GUI_SetColor(0x8e8e90);
	GUI_SetFont(&GUI_Font13_ASCII);
	sprintf(ac,"%d",12);
	GUI_DispStringHCenterAt(ac,72,294);
}