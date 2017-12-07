/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

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

#include "led.h"



//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

char jindu = 0;				//初始化进度条初始值为0

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "智能运动记录仪",    0,                       0,  0,  240,320,FRAMEWIN_CF_MOVEABLE,0},
    { CHECKBOX_CreateIndirect,  "左指向灯",          GUI_ID_CHECK0,           17, 6,  100,16, 0,0},
    { CHECKBOX_CreateIndirect,  "右指向灯",          GUI_ID_CHECK1,           17, 26, 100,16, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            117,6,  100,36, 0,0},
    { PROGBAR_CreateIndirect,    NULL,               GUI_ID_PROGBAR0,         17, 126,200,30, 0,0},
    { SLIDER_CreateIndirect,     NULL,               GUI_ID_SLIDER0,          17, 76, 200,30, 0,0},
    { SCROLLBAR_CreateIndirect,  NULL,               GUI_ID_SCROLLBAR0,       17, 176,200,30, 0,0},
    { BUTTON_CreateIndirect,    "增加",              GUI_ID_BUTTON0,          17, 236,50, 35, 0,0},
    { BUTTON_CreateIndirect,    "减小",              GUI_ID_BUTTON1,          167,236,50, 35, 0,0},
    { BUTTON_CreateIndirect,    "反转",              GUI_ID_BUTTON2,          93, 236,50, 35, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetFont(hWin,&GUI_FontHZ16);
//    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
//    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
//    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    FRAMEWIN_SetTitleHeight(hWin,20);
    //
    //GUI_ID_CHECK0
    //
    CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK0),&GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK0),"左指向灯");
    //
    //GUI_ID_CHECK1
    //
    CHECKBOX_SetFont(WM_GetDialogItem(hWin,GUI_ID_CHECK1),&GUI_FontHZ16);
    CHECKBOX_SetText(WM_GetDialogItem(hWin,GUI_ID_CHECK1),"右指向灯");
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_FontHZ16);
    EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),"显示测试");
    //
    //GUI_ID_PROGBAR0
    //
    PROGBAR_SetText(WM_GetDialogItem(hWin,GUI_ID_PROGBAR0),"");
    //
    //GUI_ID_BUTTON0
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_FontHZ16);
    //
    //GUI_ID_BUTTON1
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_FontHZ16);
    //
    //GUI_ID_BUTTON2
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),&GUI_FontHZ16);

}




/*********************************************************************
*
*       Dialog callback routine
*/
int led_flag = 0;

static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
				case GUI_ID_BUTTON0:
					jindu += 1;
					if(jindu > 99)
						jindu = 100;					
					PROGBAR_SetValue(WM_GetDialogItem(hWin,GUI_ID_PROGBAR0),jindu);
					break;
				case GUI_ID_BUTTON1:
					if(jindu < 1)
						jindu = 0;	
					else jindu -= 1;														
					PROGBAR_SetValue(WM_GetDialogItem(hWin,GUI_ID_PROGBAR0),jindu);
					break;
				case GUI_ID_BUTTON2:
					if(led_flag == 0){
						LED0 = 0;
						LED1 = 0;
						CHECKBOX_Check(WM_GetDialogItem(hWin,GUI_ID_CHECK0));
						CHECKBOX_Check(WM_GetDialogItem(hWin,GUI_ID_CHECK1));
					}
					if(led_flag == 2){
						LED0 = 1;
						LED1 = 1;
						CHECKBOX_Uncheck(WM_GetDialogItem(hWin,GUI_ID_CHECK0));
						CHECKBOX_Uncheck(WM_GetDialogItem(hWin,GUI_ID_CHECK1));
					}
					led_flag++;
					if(led_flag >= 4)
						led_flag = 0;
					break;
				case GUI_ID_CHECK0:
					if(CHECKBOX_IsChecked(WM_GetDialogItem(hWin,GUI_ID_CHECK0)))
						LED0 = 0;
					else LED0 = 1;
					break;
				case GUI_ID_CHECK1:
					if(CHECKBOX_IsChecked(WM_GetDialogItem(hWin,GUI_ID_CHECK1)))
						LED1 = 0;
					else LED1 = 1;
					break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void Task1(void) 
{ 
//    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	LED_Init();

	while(1)
	{
        GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
	}
}

