#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "window.h"
#include "button.h"
#include "edit.h"
#include "static.h"



Window * CreateWindow(Rectangle *prect, unsigned char *ppicstr, EventHandler pcallback)
{
    if(prect == NULL)
        return NULL;

    Window *pwin = malloc(sizeof(Window));

	if(pwin == NULL)
        return NULL;
	
    if(ppicstr == NULL)
	{
		printf("background pic cannot be NULL\n");
        return NULL;
    }

    memset(pwin, 0, sizeof(Window));
    memcpy(&pwin->baseControl.rect, prect, sizeof(Rectangle));
    pwin->baseControl.visible = 1;
    pwin->baseControl.eventcallback = pcallback;

	pwin->baseControl.keyeventcallback=NULL;
	pwin->baseControl.PreInit=NULL;
	pwin->baseControl.parent=NULL;
	pwin->pfirstcontrollist=NULL;

    strcpy(pwin->picstr, ppicstr);
	
    return pwin;
}

int AddControlToWindow(Window *win, ControlType contype, void *pcontrol)
{
    if(win == NULL || pcontrol == NULL)
        return 1;
    
    ControlsList *plastlist = win->pfirstcontrollist;
    ControlsList *pcurlist  = win->pfirstcontrollist;
    
    while(pcurlist != NULL)
    {
        plastlist = pcurlist;
        pcurlist = pcurlist->pnext;
    }

    ControlsList *preallist = malloc(sizeof(ControlsList));
    if(preallist == NULL)
        return 2;

    memset(preallist, 0, sizeof(ControlsList));
    preallist->contype = contype;
    preallist->pcontrol = pcontrol;

    if(win->pfirstcontrollist == NULL)
    {
        win->pfirstcontrollist = preallist;
    }
    else
    {
        plastlist->pnext = preallist;
        preallist->pprev = plastlist;
    }
    
    return 0;
}


void* GetControlByPoint(unsigned int *pcontrolbase, Window *win ,int x,int y)
{	
	BaseControl *pbaseinfo;
	
	if(win == NULL || win->pfirstcontrollist == NULL)
		return NULL;

	ControlsList *pCtlNext = win->pfirstcontrollist;

	while(pCtlNext != NULL)
	{
		pbaseinfo = NULL;
		switch(pCtlNext->contype)
		{
			case WIN_WINDOW:			
				pbaseinfo = &((Window*)pCtlNext->pcontrol)->baseControl;
				break;
				
			case WIN_STATIC:
				pbaseinfo = &((Static*)pCtlNext->pcontrol)->baseControl;
				break;
			
		   case WIN_EDIT:				   
		   		pbaseinfo = &((Edit*)pCtlNext->pcontrol)->baseControl;
				break;
			
		   case WIN_BUTTON: 
		   		pbaseinfo = &((Button*)pCtlNext->pcontrol)->baseControl;
				break;
				
		   default :	
				break;
		}
		
		if(pbaseinfo == NULL)
		{
			pCtlNext = pCtlNext->pnext;
			continue;
		}
		
		if(IsPointInControl( x, y, &pbaseinfo->rect))
		{
		//考虑控件叠加问题
			if(pbaseinfo->eventcallback == NULL)
			{
				pCtlNext = pCtlNext->pnext;
				continue;
			}
			
			if(pcontrolbase != NULL)
				*pcontrolbase = pbaseinfo;
			
			return pCtlNext->pcontrol;
		}
		
		pCtlNext = pCtlNext->pnext;
	}

	return NULL;
}



int DrawWindow(Window *win)
{
    int rvalue;
    if(win == NULL)
        return 1;

    if(win->baseControl.visible)
    {
        if(strlen(win->picstr) > 0)
        {
            rvalue = DrawJpegControl(win->picstr, win->baseControl.rect.x, win->baseControl.rect.y);
            if(rvalue)
            {
                printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
                return 2;
            }
        }
    }

   return 0;
}


int SetWindowKeyEvent(Window *win,EventHandler pkeycallback)
{
    if( NULL == pkeycallback || NULL==win)
        return 1;
	
	win->baseControl.keyeventcallback=pkeycallback;
	    return 0;

}


int SetWindowPicstr(Window *win, char *picstr)
{
    if(win == NULL||strlen(picstr)==0)
        return 1;

    memset(win->picstr, 0, sizeof(win->picstr));
    
    if(picstr != NULL)
        strcpy(win->picstr, picstr);
      
    return 0;
}

int SetWindowVisible(Window *win, unsigned char visible)
{
    if(win == NULL)
        return 1;
    
    win->baseControl.visible = visible;
    return 0;
}


int DrawWindowAndControls(Window *win)
{
	
    int rvalue;
    if(win == NULL)
        return 1;

    if(win->baseControl.visible == 0)
        return 2;
    
    ControlsList *pcurlist = win->pfirstcontrollist;
	

    //绘制windows
    rvalue = DrawWindow(win);
    if(rvalue)
    {
        printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
        return 3;
    }

    //绘制controls
    while(pcurlist != NULL)
    {
        rvalue = -1;

        if(pcurlist->contype == WIN_BUTTON)
             rvalue = DrawButton((Button*)pcurlist->pcontrol);
		
        else if(pcurlist->contype == WIN_EDIT)
             rvalue = DrawEdit((Edit*)pcurlist->pcontrol);
			 
		else if(pcurlist->contype == WIN_STATIC)
	         rvalue = DrawStatic((Static*)pcurlist->pcontrol);
	    	        
     
        if(rvalue)
        {
            printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
            //return 4;
        }
        pcurlist = pcurlist->pnext;
    }
}

int DrawWindowControls(Window *win)
{
	
    int rvalue;
    if(win == NULL)
        return 1;

    if(win->baseControl.visible == 0)
        return 2;
    
    ControlsList *pcurlist = win->pfirstcontrollist;
	

    //绘制windows

	/*
    rvalue = DrawWindow(win);
    if(rvalue)
    {
        printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
        return 3;
    }
    */

    //绘制controls
    while(pcurlist != NULL)
    {
        rvalue = -1;

        if(pcurlist->contype == WIN_BUTTON)
             rvalue = DrawButton((Button*)pcurlist->pcontrol);
		
        else if(pcurlist->contype == WIN_EDIT)
             rvalue = DrawEdit((Edit*)pcurlist->pcontrol);
			 
		else if(pcurlist->contype == WIN_STATIC)
	         rvalue = DrawStatic((Static*)pcurlist->pcontrol);
        
     
        if(rvalue)
        {
            printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
            //return 4;
        }
        pcurlist = pcurlist->pnext;
    }
}



int DestroyWindow(Window *win)
{
    if(win == NULL)
        return 1;
    free(win);
	win = NULL;
    return 0;
}


int DestroyWindowAndControls(Window *win)
{
    int rvalue;
    if(win == NULL)
        return 1;
    
    ControlsList *pcurlist = win->pfirstcontrollist;
    ControlsList *plastlist = win->pfirstcontrollist;

    while(pcurlist != NULL)
    {
        plastlist = pcurlist;
        pcurlist = pcurlist->pnext;
    }

    pcurlist = plastlist;
    
    //删除controls
    while(pcurlist != NULL)
    {
        rvalue = 10;

        if(pcurlist->contype == WIN_BUTTON)
             rvalue = DestroyButton(pcurlist->pcontrol);

        else if(pcurlist->contype == WIN_EDIT)
             rvalue = DestroyEdit(pcurlist->pcontrol);
		
	    else if(pcurlist->contype == WIN_STATIC)
			rvalue = DestroyStatic(pcurlist->pcontrol);

        if(rvalue)
        {
            printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
            return 2;
        }

        plastlist = pcurlist->pprev;
        if(plastlist!= NULL)
            plastlist->pnext = NULL;

        free(pcurlist);         
        pcurlist = plastlist;
    }

     //删除windows
    rvalue = DestroyWindow(win);
    if(rvalue)
    {
        printf("%s %d DrawWindow error %d\n",__FUNCTION__,__LINE__, rvalue);
        return 3;
    }

    return 0;
}

