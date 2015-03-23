
#ifndef _COMM_DRAWFONT_H
#define _COMM_DRAWFONT_H
#include <stdbool.h>
#include "winbase.h"


#define COLOR_WHITE 	0xffffff	//��ɫ
#define COLOR_BLACK 	0x000000	//��
#define COLOR_GRAY 		0x808080	//��ɫ
#define COLOR_RED		0xff0000	//��ɫ
#define COLOR_GREEN 	0x00ff00	//��ɫ
#define COLOR_BLUE 		0x0000ff	//��ɫ
#define COLOR_YELLOW 	0xffff00	//��ɫ
#define COLOR_CYAN 		0x00ffff	//��ɫ

#define FONT12		12      //12*12����
#define FONT16		16		//16*16����
#define FONT24		24      //24*24����


int DrawString(int x,int y,unsigned char *string,unsigned char fontsize,unsigned int strcolor);

//bool DrawString2(int x,int y,unsigned char *string,int fontsize,unsigned int strcolor, WinBaseBufinfo *pbufinfo);
unsigned int StrDisplayLen(unsigned char *string,unsigned char fontsize);
int FreeFontBuf(void);

#endif



