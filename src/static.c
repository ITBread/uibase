#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "static.h"
#include "drawfont.h"


Static *CreateStatic(Window *win, Rectangle *prect, char *pstr,unsigned char fontsize,unsigned int fontcolor, EventHandler pcallback)
{
    if(prect == NULL)
        return NULL;
   
    Static *pcontrol = malloc(sizeof(Static));
    if(pcontrol == NULL)
        return NULL;

    memset(pcontrol, 0, sizeof(Static));

    pcontrol->baseControl.visible = 1;
    pcontrol->baseControl.eventcallback = pcallback;
	pcontrol->baseControl.parent = (void*)win;
	pcontrol->textAlign = LEFT;
    memcpy(&pcontrol->baseControl.rect, prect, sizeof(Rectangle));
   
    if(pstr != NULL)
        strcpy(pcontrol->string, pstr);
	fontsize = ((fontsize==0) ? 16 : fontsize);
	pcontrol->fontsize = fontsize;
	//0±íÊ¾ºÚÉ«
	fontcolor = ((fontcolor==0) ? 0xffffff : fontcolor);
	pcontrol->fontcolor = fontcolor;

    if(AddControlToWindow(win, WIN_STATIC, pcontrol))
        return NULL;
    
	return pcontrol;
}

int DrawStatic(Static *control)
{
	int rvalue = -1, len = 0, fontx = 0,fonty = 0;
	
	if(control == NULL)
		return 1;
    if(control->baseControl.visible)
    {
        if(strlen(control->string) > 0)
        {
            len = StrDisplayLen(control->string,control->fontsize);
			
			fonty = control->baseControl.rect.y+(control->baseControl.rect.height-control->fontsize)/2;
			if(control->textAlign == LEFT)
			{	
				fontx = control->baseControl.rect.x + control->fontsize; 
			}		
			else if(control->textAlign == MIDDLE)
			{
				fontx = control->baseControl.rect.x+(control->baseControl.rect.width-len)/2;	
			}
			else if(control->textAlign == RIGHT)
			{	
				fontx = control->baseControl.rect.x+control->baseControl.rect.width-len;
			}else{
				;
			}
			
			rvalue = DrawString(fontx,fonty,control->string,control->fontsize,control->fontcolor);
            if(rvalue)
            {
                printf("%s %d DrawEdit_Draw_String error %d \n",__FUNCTION__,__LINE__,rvalue);
                return 2;
            }
        }     
    }

    return 0;
}


int SetStaticVisible(Static *control, unsigned char visible)
{
    if(control == NULL)
        return 1;
    
    control->baseControl.visible = visible;
    return 0;
}

int SetStaticString(Static *control, char *pstr)
{
    if(control == NULL)
        return 1;

    memset(control->string, 0, sizeof(control->string));
    
    if(pstr != NULL)
        strcpy(control->string, pstr);
   
    return 0;
}

int SetStaticTextAlign(Static *control, TextAlign textAlign)
{
    if(control == NULL)
        return 1;
    
    control->textAlign= textAlign;
    return 0;

}

int DestroyStatic(Static *control)
{
    if(control == NULL )
        return 1;

    free(control);
	control = NULL;
    return 0;
}


