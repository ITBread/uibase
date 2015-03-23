#ifndef    	_CONTROL_STATIC_H
#define    	_CONTROL_STATIC_H

#include "winbase.h"
#include "window.h"

typedef struct  _CStaticInfo
{	
    BaseControl baseControl;
	char string[128];
	TextAlign textAlign;
	unsigned char fontsize;
	unsigned int fontcolor;
}Static,*StaticPtr;

Static *CreateStatic(Window *win,Rectangle *prect, char *pstr,unsigned char fontsize,unsigned int fontcolor, EventHandler pcallback);

int DrawStatic(Static *control);

int SetStaticVisible(Static *control, unsigned char visible);

int SetStaticString(Static *control, char *pstr);
int SetStaticTextAlign(Static *control, TextAlign textAlign);

int DestroyStatic(Static *control);


#endif
