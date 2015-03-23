#ifndef    	_CONTROL_EDIT_H
#define    	_CONTROL_EDIT_H

#include "winbase.h"
#include "window.h"

typedef struct  _CEditInfo
{	
    BaseControl baseControl;
    char string[128];
	TextAlign textAlign;
	unsigned char fontsize;
	unsigned int fontcolor;
	unsigned char status; //0:未被选中, 1:已被选中
    unsigned char passflag;
    char picstr[128];
    
}Edit,*EditPtr;

Edit *CreateEdit(Window *win, Rectangle *prect, char *pstr,unsigned char fontsize,unsigned int fontcolor, unsigned char passflag, char *picstr, EventHandler pcallback);


int SetEditText(Edit *control, char *pstr);

int SetEditVisible(Edit *control, unsigned char visible);

//TextAlign的值 (LEFT,MIDDLE,RIGHT)
int SetEditTextAlign(Edit *control, TextAlign textAlign);

int SetEditPicstr(Edit *control, char *picstr);

int SetEditPwdFlag(Edit *control, unsigned char passflag);


int DrawEdit(Edit *control);

int DestroyEdit(Edit *control);

bool IsPointInEdit(int x,int y,Edit *edit);


#endif
