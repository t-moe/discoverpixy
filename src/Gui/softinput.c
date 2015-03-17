#include "../settings.h"
#include "../types.h"
#include "../tft/tft.h"
#include "../touch/touch.h"
#include "softinput.h"
#include <stdlib.h>

#define C1 HEX(0xCCCC99)
#define C2 HEX(0x6A6A5A)
#define C3 HEX(0x013366)
#define C4 WHITE

 char * allnums []  ={
	"1","2","3","4","5","6","7","8","9","0","",
	""
	};

 char * allmonth []={
 "Jan","Feb","Mär","Apr","Mai","Jun","",
 "Jul","Aug","Sep","Okt","Nov","Dez","",
 ""};   	

TOUCH_AREA_STRUCT* softkey_touchareas;
SOFTINPUT_CALLBACK user_cb;
 char ** user_buf;

void softkeys_cb(void* touchArea, TOUCH_ACTION triggeredAction)
{
	TOUCH_AREA_STRUCT* area = (TOUCH_AREA_STRUCT*) touchArea;
	
	switch(triggeredAction)
	
	{
		case PEN_DOWN:
			area->hookedActions=PEN_UP|PEN_LEAVE;
			tftDrawLine(area->x1,area->y1,area->x2,area->y1,C2); //Nord
			tftDrawLine(area->x1,area->y1,area->x1,area->y2,C2);//West
			tftDrawLine(area->x1,area->y2,area->x2,area->y2,C1); //Süd
			tftDrawLine(area->x2,area->y1,area->x2,area->y2,C1);  //Ost	
		break;
		case PEN_UP:
		{
			unsigned char unspaced=((unsigned int)area-(unsigned int)softkey_touchareas)/sizeof(TOUCH_AREA_STRUCT);
			unsigned char spaced=0xFF;
			unsigned char i=0;
			while(i++<=unspaced )
			{
				if(user_buf[++spaced][0]==0)
					spaced++;	
			}
			user_cb( user_buf, unspaced,spaced);
		}			
		case PEN_LEAVE:
			area->hookedActions=PEN_DOWN;
			tftDrawLine(area->x1,area->y1,area->x2,area->y1,C1); //Nord
			tftDrawLine(area->x1,area->y1,area->x1,area->y2,C1);//West
			tftDrawLine(area->x1,area->y2,area->x2,area->y2,C2); //Süd
			tftDrawLine(area->x2,area->y1,area->x2,area->y2,C2);  //Ost	
		break;
		default:break;
	}
}

void guiAddSoftInput(char ** arr,unsigned char h, SOFTINPUT_CALLBACK cb)
{
	unsigned char elem_x=0;
	unsigned char elem_y=0;
	unsigned char cnt=0;
	unsigned char cnt_x=0;
	while(1)
	{
		if(arr[cnt][0]==0)
		{
			if(cnt_x==0)
				break;
			if(cnt_x>elem_x)
				elem_x=cnt_x;
			cnt_x=0;
			elem_y++;
		}
		else 
			cnt_x++;
		cnt++;	
	}
	cnt-=elem_y;
	elem_x++;
	elem_y++;
	softkey_touchareas = malloc(sizeof(TOUCH_AREA_STRUCT)*cnt);
	user_cb = cb;
	user_buf=arr;
	
	
	if(h==AUTO)
		h= 20 * elem_y;
	unsigned int y =  DHEIGHT-h;
	unsigned char width = WIDTH/elem_x;
	unsigned char wspace = (width)/elem_x;
	unsigned char height = h/elem_y;
	unsigned char hspace = (height)/ elem_y;
	unsigned char hstart = y+1+hspace;
	
	tftDrawLine(0,y,DWIDTH,y,C3);
	tftFillRectangle(0,y+1,DWIDTH,DHEIGHT,C4);
	cnt=0;
	unsigned char cnt_txt=0;
	unsigned char cnt_areas=0;
	for(unsigned char cnt_y=0;  cnt_y<(elem_y-1); cnt_y++)
	{
		unsigned char elem_row=0;
		while(arr[cnt++][0]!=0)
			elem_row++;
		unsigned char spaceleft=(WIDTH-elem_row*width-(elem_row-1)*wspace)/2;
		for(cnt_x=0; cnt_x<elem_row; cnt_x++)
		{
			unsigned int x1 = spaceleft+cnt_x*(width+wspace);
			unsigned char y1 = hstart+cnt_y*(height+hspace);
			unsigned int x2 = x1+width;
			unsigned char y2= y1+height;
			tftPrint((width-tftPrintCalcWidth(BigFont,arr[cnt_txt]))/2+x1,(height-tftPrintCalcHeight(BigFont))/2+y1,C3,C4,BigFont,arr[cnt_txt]);
			tftDrawLine(x1,y1,x2,y1,C1); //Nord
			tftDrawLine(x1,y1,x1,y2,C1);//West
			tftDrawLine(x1,y2,x2,y2,C2); //Süd
			tftDrawLine(x2,y1,x2,y2,C2);  //Ost	
			softkey_touchareas[cnt_areas].x1=x1;
			softkey_touchareas[cnt_areas].y1=y1;
			softkey_touchareas[cnt_areas].x2=x2;
			softkey_touchareas[cnt_areas].y2=y2;
			softkey_touchareas[cnt_areas].hookedActions=PEN_DOWN;
			softkey_touchareas[cnt_areas].callback = softkeys_cb;
			touchRegisterArea(&softkey_touchareas[cnt_areas]);
			cnt_txt++;
			cnt_areas++;
		}
		cnt_txt++;
	}	
}
void guiRemoveSoftInput()
{

	unsigned char cnt=0;
	unsigned char cnt_area=0;
	while(1)
	{
		if(user_buf[cnt][0]!=0) touchUnregisterArea(&softkey_touchareas[cnt_area++]);
		else if(user_buf[cnt+1][0]==0) break;
		cnt++;	
	}		
	free((void*)softkey_touchareas);
}
void guiUpdateSoftInput()
{
	unsigned char cnt=0;
	unsigned char cnt_area=0;
	while(1)
	{
		if(user_buf[cnt][0]!=0)
		{
			unsigned int x1 = softkey_touchareas[cnt_area].x1;
			unsigned char y1 = softkey_touchareas[cnt_area].y1;
			unsigned int x2 = softkey_touchareas[cnt_area].x2;
			unsigned char y2= softkey_touchareas[cnt_area].y2;
			tftPrint(((x2-x1)-tftPrintCalcWidth(BigFont,user_buf[cnt]))/2+x1,((y2-y1)-tftPrintCalcHeight(BigFont))/2+y1,C3,C4,BigFont,user_buf[cnt]);
			cnt_area++;
		}
		else if(user_buf[cnt+1][0]==0) break;
		cnt++;	
	}
}