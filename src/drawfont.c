#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "drawfont.h"

static unsigned char  *FONTBUF[6];

static int CheckOpenAndLoadFont( char *fontpath,  int index)
{
	long int len;
	FILE *fpfont;
	if(index>5||index<0)
	{  
		printf("index is too big !\n");  
		return 5;  
	}
	
	if(FONTBUF[index] != NULL)
		return 1;

	if ((fpfont = fopen(fontpath, "rb")) == NULL )
	{  
		printf("can't open font  %s!\n",fontpath);  
		return 3;  
	} 

	fseek(fpfont,0L,SEEK_END);
	len = ftell(fpfont);
	
	FONTBUF[index] = (unsigned char *)malloc(len);
	if(FONTBUF[index] == NULL)
	{  
		printf("can't malloc font buf\n");  
		return 4;  
	} 
	
	fseek(fpfont,0L,SEEK_SET);
	fread(FONTBUF[index],len,1,fpfont);
	fclose(fpfont);
	
	return 0;

}

static int LoadFontFileToBuf(unsigned char fontsize)
{
	char fullfontname[128];
	switch(fontsize)
	{
		case 12:
			GetFontPath(fullfontname,"HZK12");
			CheckOpenAndLoadFont(fullfontname, 0);
			
			GetFontPath(fullfontname,"ASC12");
			CheckOpenAndLoadFont(fullfontname, 1);
			break;
			
		case 16:
			GetFontPath(fullfontname,"HZK16");
			CheckOpenAndLoadFont(fullfontname, 2);
			
			GetFontPath(fullfontname,"ASC16");
			CheckOpenAndLoadFont(fullfontname, 3);
			break;
			
		case 24:
			GetFontPath(fullfontname,"HZK24");
			CheckOpenAndLoadFont(fullfontname, 4);
			
			GetFontPath(fullfontname,"ASC24");
			CheckOpenAndLoadFont(fullfontname, 5);
			break;

		default:
			printf("fontsize %d error!\n", fontsize);
			return -1;	
	}
	
	return 0;
}

/*设置显示字体的颜色*/
static unsigned short SetTextColor(unsigned int textcolor)
{
    unsigned short colorrgb565;    
    unsigned char red   = (textcolor >> (16+3)) & 0x1f;
    unsigned char green = (textcolor >> (8+2)) & 0x3f;
    unsigned char blue  = (textcolor >> 3) & 0x1f;
    
    colorrgb565 = (red << 11) | (green << 5) | blue;  
    return colorrgb565;
}

/*绘制大小为12*12的中文字符*/
static void DrawText12(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{
	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<12;i++)
	{
		mask =0x80;			//掩码
	
		buffer =ch[i*2];	//提取一行的第一个字节
		
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask >>1;
		}
		
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{	
				memcpy(pdstbuf+(j+8)*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;	
		}
			
	}
}


/*绘制大小为16*16的中文字符*/
static void DrawText16(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{
	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<16;i++)
	{
		mask =0x80;			//掩码
		buffer = ch[i*2];		//提取一行的第一个字节
		
		for(j=0;j<8;j++)
		{			
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask >>1;
		}
		mask =0x80;
		buffer = ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+(j+8)*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;	
		}
			
	}
}

/*绘制大小为24*24的中文字符*/
static void DrawText24(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{
	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<24;i++)
	{
		mask =0x80;			//掩码
		buffer =ch[i*3];		//提取一行的第一个字节
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask >>1;
		}
		mask =0x80;
		buffer =ch[i*3+1];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+(j+8)*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;	
		}

		mask =0x80;
		buffer =ch[i*3+2];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+(j+16)*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;	
		}
	}
}

/*绘制大小为12*12的ASCII码*/
static void DrawASCII12(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{

	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<12;i++)
	{
		mask =0x80;
		buffer =ch[i];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;
		}
	}
	



}

/*绘制大小为8*16的ASCII码*/
static void DrawASCII16(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{
	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<16;i++)
	{
		mask = 0x80;
		buffer = ch[i];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;
		}
	}
}

/*绘制大小为12*24的ASCII码*/
static void DrawASCII24(unsigned char *pdstbuf, int stride,unsigned int strcolor,const unsigned char ch[])
{
	unsigned short i,j,textcolor;
	unsigned char mask,buffer;
	textcolor = SetTextColor(strcolor);
	for(i=0;i<24;i++)
	{
		mask =0x80;			//掩码
		buffer =ch[i*2];		//提取一行的第一个字节
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+j*2+i*stride, &textcolor, 2);
			}
			mask = mask >>1;
		}
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer & mask)
			{
				memcpy(pdstbuf+(j+8)*2+i*stride, &textcolor, 2);
			}
			mask = mask>>1;	
		}
	}
}

/*绘制字符*/
int DrawString(int x,int y,unsigned char *string,unsigned char fontsize,unsigned int strcolor)
{	
	
	WinBaseBufInfo *pbufinfo = GetWinBaseBufInfo();
	 //返回值
	int rvalue = 0;
	int num,k,disx;
	unsigned int offset;  	
	unsigned char areaCode,locCode;
	
	unsigned char buf[128];
	unsigned char *dstbuf = NULL;
	disx = 0;
	if((rvalue = LoadFontFileToBuf(fontsize)) != 0 ){
		printf("Load font file to buffer failed!\n");
		return -1;	
	}
	
	for(k=0; k<strlen(string); k++)
	{
		switch(fontsize)
		{
			case 12:
				if(string[k]&0x80){                 //中文字符
					areaCode = string[k]-0xa0;		//区号
					locCode = string[k+1]-0xa0;		//位号
					num = 24;
					offset = ((areaCode -1) * 94 + locCode - 1) * num;
					memcpy(buf,FONTBUF[0]+offset, num);
					
					if(x+disx+12 >= pbufinfo->width)
						break;
					
					dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;
					DrawText12(dstbuf, pbufinfo->stride,strcolor,buf);
					disx += 12;
					k++;
				}else{
					num = 12;
					offset = (string[k]-32) * num;
					memcpy(buf,FONTBUF[1]+offset, num);
					
					if(x+disx+8 >= pbufinfo->width)
						break;
				
		            dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;
					DrawASCII12(dstbuf, pbufinfo->stride,strcolor, buf);
					disx += 8;
				}
				break;
			
			case 16: 			
				if(string[k]&0x80){                 //中文字符
					areaCode =string[k]-0xa0;		//区号
					locCode =string[k+1]-0xa0;		//位号
					num = 32;
					offset = ((areaCode -1) * 94 + locCode - 1) * num;
					memcpy(buf,FONTBUF[2]+offset, num);
					if(x+disx+16 >= pbufinfo->width)
						break;

	        		dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;
					DrawText16(dstbuf, pbufinfo->stride,strcolor,buf);
					disx += 16;
					k++;
				}else{
					num = 16;
					offset = string[k] * num;
					memcpy(buf,FONTBUF[3]+offset, num);
					
					if(x+disx+8 >= pbufinfo->width)
						break;

		            dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;
					DrawASCII16(dstbuf, pbufinfo->stride,strcolor, buf);
					disx += 8;
				}
				break;

			case 24:  			
				if(string[k]&0x80){                 //中文字符
					areaCode =string[k]-0xa0;		//区号
					locCode =string[k+1]-0xa0;		//位号
					num = 72;
					offset = ((areaCode -1-15) * 94 + locCode - 1) * num;
					memcpy(buf,FONTBUF[4]+offset, num);
					if(x+disx+24 >= pbufinfo->width)
						break;

	        		dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;

					DrawText24(dstbuf, pbufinfo->stride,strcolor,buf);

					disx += 24;
					k++;
				}else{
					num = 48;
					offset = string[k] * num;
					memcpy(buf,FONTBUF[5]+offset, num);
					
					if(x+disx+16 >= pbufinfo->width)
						break;

		            dstbuf = pbufinfo->pdstbuf+(x+disx)*2+y*pbufinfo->stride;
					DrawASCII24(dstbuf, pbufinfo->stride,strcolor, buf);
					disx += 16;
				}
				break;
				
			default:
				printf("font %d error!\n",fontsize);
				rvalue = -2;
				break;					
		}
	}

	return rvalue;	
}

//字符个数
int StrCount(unsigned char *string)
{
	int k,hzcount = 0,asccount = 0,count = 0;
	int len;
	len = strlen(string);

	for(k=0;k<len;k++)
	{
		if(string[k]&0x80)   //中文字符
		{
			hzcount += 1;
			k += 1;
		}
		else
		{
			asccount += 1;	
		}		
	}
	count = hzcount + asccount;		
	return count;
}

//计算字符串显示在LCD上的长度(占据行像素点数)
unsigned int StrDisplayLen(unsigned char *string,unsigned char fontsize)
{
	int k,hzcount = 0,asccount = 0;
	unsigned int len,displayLen;
	len = strlen(string);

	for(k=0;k<len;k++)
	{//汉子
		if(string[k]&0x80)   
		{
			hzcount += 1;
			k += 1;
		}
		else
		{
			asccount += 1;	
		}
	}
	
	if(fontsize == 24)
		displayLen = hzcount * fontsize + asccount * 16;
	else
		displayLen = hzcount * fontsize + asccount * fontsize/2;

	return displayLen;
}

int FreeFontBuf(void)
{
	int i=0;
	for(i=0;i<6;i++)
	{
		if(FONTBUF[i]!=NULL)
		{
			free(FONTBUF[i]);
			FONTBUF[i]=NULL;
		}
	}

	return 0;
}


