#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "winbase.h"

//»º³åÇø
static WinBaseBufInfo *UIbufinfo = NULL;
/*
static char resfold[128] = "/var/images";
static char fontfold[128] = "/var/fonts";
*/
static char resfold[128] = "./images";
static char fontfold[128] = "./fonts";



int IsPointInControl(int x,int y,Rectangle *rect)
{
	if(rect!=NULL)
		return(x > rect->x && x < rect->x+rect->width && y > rect->y && y < rect->y+rect->height);
	else
		return 0;
}

WinBaseBufInfo *GetWinBaseBufInfo()
{
	return UIbufinfo;
}

int SetWinBaseBuf(unsigned char *pdstbuf, unsigned int dstpaddr, int width, int height, int stride)
{
	if(UIbufinfo==NULL)
	{
		UIbufinfo=(WinBaseBufInfo*)malloc(sizeof(WinBaseBufInfo));
		if(UIbufinfo==NULL)
		{
			printf("SetWinbaseBuf malloc error\n");
			return 0;
		}
	}
	if(pdstbuf == NULL)
	{
		printf("SetWinbaseBuf Argment is NULL\n");
		return 1;
	}

	UIbufinfo->width = width;
	UIbufinfo->height = height;

	UIbufinfo->pdstbuf = pdstbuf;
	UIbufinfo->stride = stride;
	UIbufinfo->dstpaddr = dstpaddr;

	return 0;
}

int  SetRect(Rectangle *rect,int x,int y,int width,int height)
{
	if(rect==NULL)
	{
		printf("SetRect Argment is NULL\n");
		return -1;
	}
	rect->x=x;
	rect->y=y;
	rect->width=width;
	rect->height=height;
	return 0;
}
int SetControlTag(BaseControl *control, void *tag)
{
    int rvalue;
    if(control == NULL)
        return 1;
	
	control->tag= tag;
	
    return 0;
}

void SetResFold(char *fold)
{
	strcpy(resfold,fold);
}
void SetFontFold(char *fold)
{
	strcpy(fontfold,fold);
}

int  GetFontPath(char *fullfontname,char* fontname)
{
	return 	sprintf(fullfontname, "%s/%s",fontfold, fontname);

}

int GetPicPath(char *picfullpath,char *picfold,char *picname)
{
	return sprintf(picfullpath, "%s/%s/%s",resfold, picfold, picname);
}


