#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "edit.h"
#include "drawfont.h"


Edit *CreateEdit(Window *win, Rectangle *prect, char *pstr,unsigned char fontsize,unsigned int fontcolor, unsigned char passflag, char *picstr, EventHandler pcallback)
{
    if(NULL == prect )
        return NULL;
    
    Edit *pcontrol = malloc(sizeof(Edit));
    if(NULL == pcontrol )
        return NULL;

    memset(pcontrol, 0, sizeof(Edit));

    pcontrol->baseControl.visible = 1;
    pcontrol->baseControl.eventcallback = pcallback;
	pcontrol->baseControl.parent=(void*)win;
	pcontrol->textAlign = MIDDLE;
    memcpy(&pcontrol->baseControl.rect, prect, sizeof(Rectangle));
   
    if(NULL != pstr )
        strcpy(pcontrol->string, pstr);
	
	fontsize = (fontsize == 0 ? 16 : fontsize);
	pcontrol->fontsize = fontsize;

	fontcolor = (fontcolor==0 ? 0xffffff : fontcolor);
	pcontrol->fontcolor = fontcolor;

    if(NULL != picstr )
        strcpy(pcontrol->picstr, picstr);
	
	pcontrol->passflag = passflag;

    if(AddControlToWindow(win, WIN_EDIT, pcontrol))
    {	
    	//return NULL;
		printf("CreateEdit AddControlToWindow Error\n");
	}
     
	return pcontrol;
}


int DrawEdit(Edit *control)
{
    int rvalue = -1, len = 0, fontx = 0,fonty = 0;

	if( NULL == control)
		return 1;
	
    if(control->baseControl.visible)
    {
        if(strlen(control->picstr) > 0)
        {
            rvalue = DrawJpegControl(control->picstr, control->baseControl.rect.x , control->baseControl.rect.y);
            if(rvalue)
            {
                printf("%s %d DrawEdit error %d\n",__FUNCTION__,__LINE__, rvalue);
                return 2;
            }
        }
         
        if(strlen(control->string) > 0)
        {
            char realstr[128];
            memset(realstr, 0, sizeof(realstr));
            
            if(control->passflag)
                memset(realstr, '*', strlen(control->string));
            else
               strcpy(realstr, control->string);
			
	//		printf("realstr: %s\n",realstr); //debug
		
			len = StrDisplayLen(realstr,control->fontsize);
			
			fonty = control->baseControl.rect.y+(control->baseControl.rect.height-control->fontsize)/2;
			if(LEFT == control->textAlign )	
				fontx = control->baseControl.rect.x + control->fontsize; 
			else if(MIDDLE == control->textAlign)
				fontx = control->baseControl.rect.x+(control->baseControl.rect.width-len)/2;	
			else if(RIGHT == control->textAlign )
				fontx = control->baseControl.rect.x+control->baseControl.rect.width-len;

			rvalue = DrawString(fontx,fonty,realstr,control->fontsize,control->fontcolor);
            if(rvalue)
            {
                printf("%s %d DrawEdit_Draw_String error %d \n",__FUNCTION__,__LINE__,rvalue);
                return 3;
            }
			
        }
    }

    return 0;
}


int SetEditText(Edit *control, char *pstr)
{
    int rvalue;
    if( NULL == control)
        return 1;

    memset(control->string, 0, sizeof(control->string));
    if(pstr != NULL)
        strcpy(control->string, pstr);

    return 0;
}




int SetEditVisible(Edit *control, unsigned char visible)
{
    if( NULL == control)
        return 1;
    
    control->baseControl.visible = visible;
    return 0;
}

int SetEditPwdFlag(Edit *control, unsigned char passflag)
{
    if( NULL == control)
        return 1;
    
    control->passflag= passflag;
    return 0;
}


int SetEditTextAlign(Edit *control, TextAlign textAlign)
{
    if( NULL == control)
        return 1;
    
    control->textAlign= textAlign;
    return 0;

}

int SetEditPicstr(Edit *control, char *picstr)
{
    if( NULL == control||0 == strlen(picstr))
        return 1;

    memset(control->picstr, 0, sizeof(control->picstr));
    
    if(NULL != picstr )
        strcpy(control->picstr, picstr);
      
    return 0;
}



int DestroyEdit(Edit *control)
{
     if( NULL == control)
        return 1;

    free(control);
	control = NULL;
    return 0;
}

bool IsPointInEdit(int x,int y,Edit *edit)
{
	
	return(x>=edit->baseControl.rect.x && x<=edit->baseControl.rect.x+edit->baseControl.rect.width && y>=edit->baseControl.rect.y && y<=edit->baseControl.rect.y+edit->baseControl.rect.height);
}




