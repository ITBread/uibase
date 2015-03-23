#ifndef    	_EVENTHANDER_H
#define    	_EVENTHANDER_H

#include <time.h>
#include "winbase.h"
#include "window.h"

#define MAX_GUI_MESSAGE_LEN  32*6

typedef struct _gui_message
{
	long msg_type;
	char msg_text[MAX_GUI_MESSAGE_LEN];
}gui_message;

typedef struct  _EventMsg
{	
	int x;
	int y;
	int keycode;
	int tag;
}EventMsg;

Window *GetCurrentWindow();
Window *GetPreWindow();

int IsCurrentWindow(Window *win);

int SendMessageToWindow(unsigned char cmd,int x,int y);

int InitMessageThread();

time_t GetLastTourchTime();


int ActiveWindow(Window *win);

int ActivePreWindow(Window *win);


#endif


