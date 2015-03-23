
#ifndef _COMM_DRAWFONT_H
#define _COMM_DRAWFONT_H
#include <stdbool.h>
#include "winbase.h"


#define COLOR_WHITE 	0xffffff	//白色
#define COLOR_BLACK 	0x000000	//黑
#define COLOR_GRAY 		0x808080	//灰色
#define COLOR_RED		0xff0000	//红色
#define COLOR_GREEN 	0x00ff00	//绿色
#define COLOR_BLUE 		0x0000ff	//蓝色
#define COLOR_YELLOW 	0xffff00	//黄色
#define COLOR_CYAN 		0x00ffff	//青色

#define FONT12		12      //12*12字体
#define FONT16		16		//16*16字体
#define FONT24		24      //24*24字体


int DrawString(int x,int y,unsigned char *string,unsigned char fontsize,unsigned int strcolor);

//bool DrawString2(int x,int y,unsigned char *string,int fontsize,unsigned int strcolor, WinBaseBufinfo *pbufinfo);
unsigned int StrDisplayLen(unsigned char *string,unsigned char fontsize);
int FreeFontBuf(void);

#endif



