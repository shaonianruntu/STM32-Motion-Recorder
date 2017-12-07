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

void Draw_Panel(void)
{

	int i;
	float a;//?? 
	int x,y;//???????
	char ac[2];

	int c=0;




	
	//***********************************??1**********************************//
	GUI_SetPenSize(0);
	GUI_SetColor(GUI_CYAN);
	GUI_AA_DrawArc(180,180,150,150,-30,210);//??,??,??

	GUI_SetColor(GUI_CYAN);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_DispStringAt("RPM X 100",140,150);

	GUI_SetPenSize(15);
    GUI_SetColor(0x0000AA);
    GUI_AA_DrawArc(180, 180, 142, 142, -30, 30);
	GUI_SetColor(0x00AA00);
    GUI_AA_DrawArc(180, 180, 142, 142, 30, 120);
	for(i=0;i<=120;i++)
	{
		GUI_SetColor(GUI_CYAN);
		a=(210-i*2)*3.1415926/180;
		if(i%5==0)
		{
			GUI_SetPenSize(2);
			x=-130*cos(a)+180;
			y=-130*sin(a)+180;
			GUI_AA_DrawLine(x,y,-150*cos(a)+180,-150*sin(a)+180);

			x=115*cos(a)+180;
			y=-115*sin(a)+180;
			sprintf(ac,"%d",i*2);
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_SetFont(&GUI_Font8_ASCII);
			GUI_DispStringHCenterAt(ac,x,y);

		}
		else
		{
			GUI_SetPenSize(0);
			x=-140*cos(a)+180;
			y=-140*sin(a)+180;
			GUI_AA_DrawLine(x,y,-150*cos(a)+180,-150*sin(a)+180);
		}
		if(i%10==0)
		{
			GUI_SetPenSize(1);
			x=-100*cos(a)+180;
			y=-100*sin(a)+180;
			GUI_AA_DrawLine(x,y,-90*cos(a)+180,-90*sin(a)+180);
			x=80*cos(a)+180;
			y=-80*sin(a)+180;
			sprintf(ac,"%d",i*2);
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_SetFont(&GUI_Font13B_ASCII);
			GUI_DispStringHCenterAt(ac,x,y);
		}
	}
	//***********************************??1**********************************//

	//***********************************??2**********************************//
	//??2	???
	GUI_SetPenSize(0);
	GUI_SetColor(GUI_YELLOW);
	GUI_AA_DrawArc(600,180,105,105,-60,180);//??,??,??

	GUI_SetPenSize(15);
    GUI_SetColor(0x0000AA);
    GUI_AA_DrawArc(600, 180, 152, 152, -60, 0);
	GUI_SetColor(0x00AA00);
    GUI_AA_DrawArc(600, 180, 152, 152, 0, 120);



	GUI_SetColor(GUI_YELLOW);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_DispStringAt("km / h",580,150);

	for(i=0;i<=60;i++)
	{

		a=(i*4-60)*3.1415926/180;
		

		if(i%5==0)
		{
			a=(i*4)*3.1415926/180;
			GUI_SetPenSize(2);
			x=-140*cos(a)+600;
			y=-140*sin(a)+180;
			GUI_SetColor(GUI_YELLOW);
			GUI_AA_DrawLine(x,y,-160*cos(a)+600,-160*sin(a)+180);

			a=(i*4-60)*3.1415926/180;
			x=130*cos(a)+600;
			y=-130*sin(a)+180;
			sprintf(ac,"%d",120-(i*2));
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_SetFont(&GUI_Font13B_ASCII);
			GUI_SetColor(GUI_WHITE);
			GUI_DispStringHCenterAt(ac,x,y);

		}
		else
		{
			a=(i*4)*3.1415926/180;
			GUI_SetPenSize(0);
			x=-150*cos(a)+600;
			y=-150*sin(a)+180;
			GUI_SetColor(GUI_YELLOW);
			GUI_AA_DrawLine(x,y,-160*cos(a)+600,-160*sin(a)+180);

			x=-100*cos(a)+600;
			y=-100*sin(a)+180;
			GUI_AA_DrawLine(x,y,-110*cos(a)+600,-110*sin(a)+180);
		}
		if(i%6==0)
		{
			a=(i*4-60)*3.1415926/180;
			x=80*cos(a)+600;
			y=-80*sin(a)+180;
			sprintf(ac,"%d",120-(i*2));
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_SetFont(&GUI_Font13B_ASCII);
			GUI_SetColor(GUI_YELLOW);
			GUI_DispStringHCenterAt(ac,x,y);
		}
	}
	//***********************************??2**********************************//


		

	//***********************************??3**********************************//
	//??3 ???
	//GUI_SetPenSize(0);
	//GUI_SetColor(GUI_BLUE);
	//GUI_AA_DrawArc(200,440,100,100,15,165);//??,??,??

	GUI_SetPenSize(30);
    GUI_SetColor(0x0000AA);
    GUI_AA_DrawArc(200, 440, 125, 125, 45, 60);
	GUI_SetColor(0x00AA00);
    GUI_AA_DrawArc(200, 440, 125, 125, 60, 90);
	GUI_SetPenSize(2);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_DrawArc(200, 440, 110, 110, 45, 135);
	GUI_AA_DrawArc(200, 440, 140, 140, 45, 135);

	for (i = 45; i <= 135; i += 15) 
	{
      float co = cos(i * 3.1415926 / 180);
      float si = sin(i * 3.1415926 / 180);
      int   x1 = 200 - 110 * co;
      int   y1 = 440    - 110 * si;
      int   x2 = 200 - (140 - 1) * co;
      int   y2 = 440    - (140 - 1) * si;
      int   xt = 200 - 100 * co;
      int   yt = 440    - 100 * si;
      GUI_SetColor(GUI_WHITE);
      GUI_SetPenSize(2);
      GUI_AA_DrawLine(x1, y1, x2, y2);
      GUI_SetColor(GUI_GREEN);
      GUI_SetFont(&GUI_Font8x8);
      GUI_DispCharAt('0' + c++, xt - 4, yt - 4);
    }

}
