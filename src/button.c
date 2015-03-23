#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "button.h"

Button* CreateButton(Window *win, Rectangle *prect, char *ppicstr, EventHandler pcallback)
{
    //  if(win == NULL || prect == NULL || ppicstr == NULL || pcallback == NULL)
	if(win == NULL || prect == NULL || ppicstr == NULL )
        return NULL;

    Button *pcontrol = malloc(sizeof(Button));
    if(pcontrol == NULL)
        return NULL;
        
    memset(pcontrol, 0, sizeof(Button));
    
    pcontrol->baseControl.visible = 1;
    pcontrol->baseControl.eventcallback = pcallback;
	pcontrol->baseControl.parent = (void*)win;
    memcpy(&pcontrol->baseControl.rect, prect, sizeof(Rectangle));
   
    strcpy(pcontrol->picstr, ppicstr); 
    if(AddControlToWindow(win, WIN_BUTTON, pcontrol))
    {  	//return NULL;
    	printf("CreateButton AddControlToWindow\n");
	}

	return pcontrol;
}
Button* CreateButton2(Window *win, Rectangle *prect, char *ppicstr,char *ppicstr2, EventHandler pcallback)
{
	if(win == NULL || prect == NULL || ppicstr == NULL )
        return NULL;

    Button *pcontrol = malloc(sizeof(Button));
    if(pcontrol == NULL)
        return NULL;

    memset(pcontrol, 0, sizeof(Button));

    pcontrol->baseControl.visible = 1;
    pcontrol->baseControl.eventcallback = pcallback;
	pcontrol->baseControl.parent = (void*)win;
    memcpy(&pcontrol->baseControl.rect, prect, sizeof(Rectangle));
   
    strcpy(pcontrol->picstr, ppicstr); 
	strcpy(pcontrol->picstr2, ppicstr2); 
	
    if(AddControlToWindow(win, WIN_BUTTON, pcontrol))
    {  
    	//return NULL;
    	printf("CreateButton AddControlToWindow\n");
	}

	return pcontrol;

}

int DrawButton(Button *control)
{	 
    int rvalue;
	if(NULL ==control)
		return 1;
	
    if(control->baseControl.visible)
    {
        if(strlen(control->picstr) > 0)
        {
            rvalue = DrawJpegControl(control->picstr, control->baseControl.rect.x , control->baseControl.rect.y);
            if(rvalue)
            {
                printf("%s %d DrawButton error %d\n",__FUNCTION__,__LINE__, rvalue);
                return 2;
            }
        }       
    }

    return 0;
}

int DrawButtonDownUp(Button *control)
{	 
    int rvalue;
	if(NULL ==control)
		return 1;
	
    if(control->baseControl.visible)
    {
        if(strlen(control->picstr2) > 0)
        {
            rvalue = DrawJpegControl(control->picstr2, control->baseControl.rect.x , control->baseControl.rect.y);
            if(rvalue)
            {
                printf("%s %d DrawButton error %d\n",__FUNCTION__,__LINE__, rvalue);
                return 2;
            }
        }
		usleep(5*10000);
		DrawButton(control);

		
    }

    return 0;
}

int SetButtonVisible(Button *control, unsigned char visible)
{
    if(control == NULL)
        return 1;
    
    control->baseControl.visible = visible;
    return 0;
}

int SetButtonPicstr(Button *control, char *picstr)
{
    if(control == NULL||strlen(picstr)==0)
        return 1;

    memset(control->picstr, 0, sizeof(control->picstr));
    
    if(picstr != NULL)
        strcpy(control->picstr, picstr);
      
    return 0;
}

int SetButtonPicstr2(Button *control, char *picstr2)
{
    if(control == NULL||strlen(picstr2)==0)
        return 1;

    memset(control->picstr2, 0, sizeof(control->picstr2));
    
    if(picstr2 != NULL)
        strcpy(control->picstr2, picstr2);
      
    return 0;
}


int DestroyButton(Button *control)
{
    if(control == NULL )
        return 1;

    free(control);
	control = NULL;
    return 0;
}

bool IsPointInButton(int x,int y,Button *btn)
{
	return(x>=btn->baseControl.rect.x && x<=btn->baseControl.rect.x+btn->baseControl.rect.width && y>=btn->baseControl.rect.y && y<=btn->baseControl.rect.y+btn->baseControl.rect.height);
}



