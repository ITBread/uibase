#ifndef    	_CONTROL_WINBASE_H
#define    	_CONTROL_WINBASE_H
#include <stdbool.h>

typedef  void(*EventHandNoPara)();
typedef  void(*EventHandOnePara)(void *object);
//typedef  void(*eventHand)(void *object, void *msg);
typedef  void(*EventHandler)(void *object, void *msg);
typedef  int (*MatchHandler )(void *object1, void *object2);

typedef struct _Rectangle 
{
	int x;	
	int y;
	int width;
	int height;	
} Rectangle,*RectanglePtr;

//�ؼ�����
typedef enum
{
	WIN_NULL=0,				// NUll
	WIN_WINDOW,				// window
	WIN_STATIC,				// static
	WIN_EDIT,				// edit
	WIN_BUTTON				// button

}ControlType;

typedef enum
{
	LEFT=0,  
	MIDDLE,
	RIGHT,   
	TOP,  	  
	BOTTOM  
}TextAlign;

typedef struct _WinBaseBufInfo
{
	//�����ڴ���ʼ��ַ
	unsigned char 	*pdstbuf;
	unsigned int 	dstpaddr;
	int 			width;
	int 			height;
	//һ�е��ڴ泤��
  	int 			stride;
}WinBaseBufInfo,*WinBaseBufInfoPtr;

typedef struct _BaseControl
{
    Rectangle   	rect;
    unsigned char   visible;
	unsigned char   enable;
	
    EventHandler   	eventcallback;
	EventHandler   	keyeventcallback;
	EventHandler	PreInit;
	void* parent;
	void*  tag;
}BaseControl,*BaseControlPtr;

//����ڵ�
typedef struct  ControlsList
{
	ControlType contype;
	void *pcontrol;
	struct ControlsList *pnext;
	struct ControlsList *pprev;    
}ControlsList;


int IsPointInControl(int x,int y,Rectangle *rect);

WinBaseBufInfo *GetWinBaseBufInfo();

int SetWinBaseBuf(unsigned char 	*pdstbuf, unsigned int dstpaddr, int width,int height,int stride);

int SetControlTag(BaseControl *control, void *tag);

int SetRect(Rectangle *rect,int x,int y,int width,int height);

int GetPicPath(char *picfullpath,char *picfold,char *picname);

void SetResFold(char *fold);

void SetFontFold(char *fold);

int GetFontPath(char *fullfontname,char* fontname);


#endif


