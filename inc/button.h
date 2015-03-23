#ifndef    	_CONTROL_BUTTON_H
#define    	_CONTROL_BUTTON_H

#include  "winbase.h"
#include  "window.h"


// button  ¥∞ø⁄ Ù–‘
typedef struct  _CButtonInfo
{	
	BaseControl baseControl;
	char picstr[128];
	char picstr2[128];
	
}Button,*ButtonPtr;


Button* CreateButton(Window *win, Rectangle *prect, char *ppicstr, EventHandler pcallback);
Button* CreateButton2(Window *win, Rectangle *prect, char *ppicstr,char *ppicstr2, EventHandler pcallback);

int DrawButton(Button *control);

int DrawButtonDownUp(Button *control);

int SetButtonVisible(Button *control, unsigned char visible);

int SetButtonPicstr(Button *control, char *picstr);
int SetButtonPicstr2(Button *control, char *picstr2);

int DestroyButton(Button *control);

bool IsPointInButton(int x,int y,Button *btn);



#endif
