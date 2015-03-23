#ifndef    	_CONTROL_WINDOW_H
#define    	_CONTROL_WINDOW_H

#include "winbase.h"

typedef struct  _CWindowInfo
{
    char picstr[128];
    BaseControl baseControl;
 	//¿Ø¼þÁ´±í
    ControlsList* pfirstcontrollist;
    
}Window,*WindowPtr;


Window* CreateWindow(Rectangle *prect, unsigned char *ppicstr, EventHandler pcallback);

int DrawWindow(Window *win);

int DrawWindowAndControls(Window *win);

int DrawWindowControls(Window *win);


int DestroyWindow(Window *win);

int SetWindowPicstr(Window *win, char *picstr);

int SetWindowVisible(Window *win, unsigned char visible);

int AddControlToWindow(Window *win, ControlType contype, void *pcontrol);

int DestroyWindowAndControls(Window *win);

int SetWindowKeyEvent(Window *win,EventHandler pkeycallback);

int CheckPonitHasControl(Window *win ,int x,int y);





#endif

