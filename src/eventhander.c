#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/prctl.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/mman.h>
#include <pthread.h>

#include "eventhander.h"
#include "button.h"
#include "edit.h"
#include "static.h"

pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

static int sysmessagehand = -1;
static unsigned char sysmessageInited = 0;

static Window *curWindow=NULL;
static Window *preWindow=NULL;
//最后触摸时间
static 	time_t lasttime;

static void CheckAndDoEvent( BaseControl *baseControl, void *object, EventMsg *msg )
{
	if( NULL == baseControl || baseControl->visible == 0)
		return ;
	
	if(IsPointInControl(msg->x,msg->y,&baseControl->rect))
	{
		if(baseControl->eventcallback != NULL && baseControl->visible == 1)
			baseControl->eventcallback(object, msg);
	}
}

static void TranslateMessage_New(Window *win,EventMsg *msg )
{
	void* pcurcontrol;
	BaseControl *pbaseControl;
	unsigned int basecontrolsaddr;

	pcurcontrol = GetControlByPoint(&basecontrolsaddr, win, msg->x, msg->y);
	if(pcurcontrol == NULL)
		return ;

	pbaseControl = (BaseControl *)basecontrolsaddr;
	if(pbaseControl->eventcallback != NULL && pbaseControl->visible==1)
		pbaseControl->eventcallback(pcurcontrol, msg);
}

static void TranslateMessage(Window *win,EventMsg *msg )
{
	ControlsList *pCtlNext = win->pfirstcontrollist;
	
	Window  winTemp;
	Static  staticTemp;
	Edit	editTemp;
	Button  btnTemp;

	BaseControl baseControl;

	if(win == NULL || win->pfirstcontrollist == NULL)
		return;

	while(pCtlNext != NULL)
	{
		switch(pCtlNext->contype)
		{
			case WIN_WINDOW:			
				memcpy(&winTemp,(Window *)pCtlNext->pcontrol,sizeof(Window));
				memcpy(&baseControl,&winTemp.baseControl,sizeof(BaseControl));
				break;
				
			case WIN_STATIC:
				memcpy(&staticTemp,(Static *)pCtlNext->pcontrol,sizeof(Static));
				memcpy(&baseControl,&staticTemp.baseControl,sizeof(BaseControl));
				break;
			
		   case WIN_EDIT:				   
		   		memcpy(&editTemp,(Edit *)pCtlNext->pcontrol,sizeof(Edit));
			    memcpy(&baseControl,&editTemp.baseControl,sizeof(BaseControl));
				break;
			
		   case WIN_BUTTON: 
		   		memcpy(&btnTemp,(Button *)pCtlNext->pcontrol,sizeof(Button));
			    memcpy(&baseControl,&btnTemp.baseControl,sizeof(BaseControl));
				break;
				
		   default :
				//printf("Can not Find Mach Control in %s \n\n",__FUNCTION__,__LINE__);			
				break;
		}
		
		CheckAndDoEvent(&baseControl,pCtlNext->pcontrol,msg);
		pCtlNext = pCtlNext->pnext;
	}
}



static void TranslateKeyEvent(Window *win,EventMsg *msg )
{
	if( NULL == win)
		return ;
	if(NULL !=win->baseControl.keyeventcallback)
		win->baseControl.keyeventcallback(win,msg);	
}

static int PreTranslateMessage(unsigned char databuf[MAX_GUI_MESSAGE_LEN], unsigned char datalen)
{

	Window *win = GetCurrentWindow();	

	unsigned int tempid;
	EventMsg msg;
	unsigned char cmd;
	cmd = databuf[0];
	
	memcpy(&tempid,databuf+1,sizeof(int));
	memcpy(&msg,databuf+1+sizeof(win),sizeof(EventMsg));

	if(tempid != (unsigned int)win || NULL == win )
			return;
	
	if(4 == cmd)
		TranslateKeyEvent(win,&msg);
	else
		TranslateMessage(win,&msg);

    return 0;
}

static void *ProcessMessageLoop(void *args)
{

  	int rvalue = 0;
	gui_message messagebuf;

	while(1) 
	{
	  	if(msgrcv(sysmessagehand, &messagebuf, MAX_GUI_MESSAGE_LEN, 0, 0) > 0)
	  	{		
			rvalue = PreTranslateMessage(messagebuf.msg_text, MAX_GUI_MESSAGE_LEN);
			if(rvalue == 0)
				continue;
	  	}
	} 
	
	return (void *)0;
}

Window *GetCurrentWindow()
{
	return curWindow;
}

Window *GetPreWindow()
{
	return preWindow;
}


int IsCurrentWindow(Window *win)
{
	if(win == curWindow && NULL != win)
		return 1;
	else
		return 0;
}

time_t GetLastTourchTime()
{	
	return lasttime;
}

int SendMessageToWindow(unsigned char cmd,int x,int y)
{
	int rvalue = 0;
	gui_message messagebuf;
	Window *win=NULL;

	messagebuf.msg_type = 1;
	messagebuf.msg_text[0] = cmd;

	//获取现在时间
	time(&lasttime);
		
	win = GetCurrentWindow();
	EventMsg msg;
	
	if( NULL == win )
		return 1;
	
	msg.x = x;
	msg.y = y;
	msg.tag=cmd;
	
	memcpy(messagebuf.msg_text+1, &win, sizeof(int));
	memcpy(messagebuf.msg_text+1+sizeof(int), &msg, sizeof(EventMsg));

	rvalue = msgsnd(sysmessagehand, &messagebuf, MAX_GUI_MESSAGE_LEN, IPC_NOWAIT);
	if(rvalue)
	{
		printf("sendmessage_to_system error %s\n", strerror(errno));
		return  2;
	}
	
	return rvalue;
}

int InitMessageThread()
{	
	if( !sysmessageInited )
	{
		int rvalue;
		pthread_t threadid;
    
		sysmessagehand = msgget(IPC_PRIVATE, IPC_CREAT|0660);
		if(sysmessagehand == -1)
		{
			printf("InitMessageThread msgget error 0x%s\n", strerror(errno));
			return 1;
		}
		rvalue = pthread_create(&threadid,NULL,ProcessMessageLoop,NULL);
		if(rvalue != 0)
		{
			printf("InitMessageThread pthread_create err 0x%s\n", strerror(errno));
			return 2;
		}
		sysmessageInited = 1;
	}
  	return 0;
}

int ActiveWindow(Window *win)
{
	pthread_mutex_lock(&mymutex);
	if(NULL !=win && curWindow != win)
	{	
		preWindow=curWindow;
		curWindow = win;

		InitMessageThread();
		DrawWindowAndControls(win);	
	}
	pthread_mutex_unlock(&mymutex); 
	
	return 0;
}

int ActivePreWindow(Window *win)
{
	if( NULL !=win )
	{	
		preWindow=curWindow;
		curWindow = win;
		
		InitMessageThread();
		DrawWindowControls(win);
		

	}
	
	return 0;
}



