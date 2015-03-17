#include "../settings.h"
#include "../types.h"
#include "tft.h"
//#include "../touch/touch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../SD/pff.h"

void _tftData(unsigned int dat)
{
	//Makro Definieren	 (Damit man über das QM (QuickMakro) sich LCALL und RET spaaren kann, auf Kosten des Flash-Speichers)
	//Funktioniert nur bis zur Optimierungsstufe 8.   Ab Stufe 9 (Common Sub Blocks) ist das Makro nutzlos.
	#define _tftDataQM(dat){ \
	SET_RS;\
	DATAPORT_LOW((dat)); \
	DATAPORT_HIGH((dat) >> 8); \
	CLEAR_WR; \
	SET_WR;}	 
	 
	_tftDataQM(dat); //Makro einsetzen	
}
void _tftCmd(unsigned char cmd)
{
	CLEAR_RS;
	DATAPORT_LOW(cmd);
	DATAPORT_HIGH(0);
	CLEAR_WR;
	SET_WR;
}
void _tftCmdData(unsigned char cmd, unsigned int dat)
{
   _tftCmd(cmd);
   _tftData(dat);
}
int ATputchar(char c,FILE*f);
void tftInit(void)
{
	DATAPORT_LOW_DDR.DIR = 0xFF;			//Low Datenport als Ausgang setzen
	DATAPORT_HIGH_DDR.DIR = 0xFF;			//High Datenport als Ausgang setzen
	PORTD.DIRSET = Display_CTRL_RESET_bm;		//Reset Pin als Ausgang setzen
	PORTE.DIRSET = Display_CTRL_RS_bm | Display_CTRL_WR_bm | Display_CTRL_RD_bm;			//RS,WR,RD als Ausgag setzen
	PORTC.DIRSET = Display_CTRL_CS_bm;			//CS als Ausgag setzen
	PORTE.OUTSET = Display_CTRL_RD_bm;			//READ setzen
	
	PORTC.DIRSET = Display_Backlight_bm;			//Backlight =Ausgang
	//PORTC.OUTSET = Display_Backlight_bm;			// = Aus
	#ifdef CONTROLLER_SSD1289
		CLEAR_RESET;
		SET_RESET;
	
		CLEAR_CS;
		_tftCmdData(0x00,0x0001);
		_tftCmdData(0x03,0xA8A4);
		_tftCmdData(0x0C,0x0000);
		_tftCmdData(0x0D,0x080C);
		_tftCmdData(0x0E,0x2B00);
		_tftCmdData(0x1E,0x00B7);
		_tftCmdData(0x01,0x2B3F);
		_tftCmdData(0x02,0x0600);
		_tftCmdData(0x10,0x0000);
		_tftCmdData(0x11,0x6018);
		_tftCmdData(0x05,0x0000);
		_tftCmdData(0x06,0x0000);
		_tftCmdData(0x16,0xEF1C);
		_tftCmdData(0x17,0x0003);
		_tftCmdData(0x07,0x0233);
		_tftCmdData(0x0B,0x0000);
		_tftCmdData(0x0F,0x0000);
		_tftCmdData(0x41,0x0000);
		_tftCmdData(0x42,0x0000);
		_tftCmdData(0x48,0x0000);
		_tftCmdData(0x49,0x013F);
		_tftCmdData(0x4A,0x0000);
		_tftCmdData(0x4B,0x0000);
		_tftCmdData(0x44,0xEF00);
		_tftCmdData(0x45,0x0000);
		_tftCmdData(0x46,0x013F);
		_tftCmdData(0x30,0x0707);
		_tftCmdData(0x31,0x0204);
		_tftCmdData(0x32,0x0204);
		_tftCmdData(0x33,0x0502);
		_tftCmdData(0x34,0x0507);
		_tftCmdData(0x35,0x0204);
		_tftCmdData(0x36,0x0204);
		_tftCmdData(0x37,0x0502);
		_tftCmdData(0x3A,0x0302);
		_tftCmdData(0x3B,0x0302);
		_tftCmdData(0x23,0x0000);
		_tftCmdData(0x24,0x0000);
		_tftCmdData(0x25,0x8000);
		_tftCmdData(0x4f,0x0000);
		_tftCmdData(0x4e,0x0000);
		_tftCmd(0x22);   
		SET_CS;
	#endif
	#ifdef CONTROLLER_ILI9327
		
		CLEAR_RESET;
		_delay_ms(1);
		SET_RESET;
		_delay_ms(1);
		
		CLEAR_CS;
		_tftCmd(0xE9);
		_tftData(0x0020);
		_delay_ms(1);
		_tftCmd(0x11); //Exit Sleep
		_delay_ms(100);
		_tftCmd(0xD1);
		_tftData(0x0000);
		_tftData(0x0071);
		_tftData(0x0019);
		_delay_ms(1);
		_tftCmd(0xD0);
		_tftData(0x0007);
		_tftData(0x0001);
		_tftData(0x0008);
		_delay_ms(1);
		_tftCmd(0x36);		//Set_address_mode (36)
		_tftData(0x0069);
		_delay_ms(1);
		_tftCmd(0x3A);
		_tftData(0x0005);
		_delay_ms(1);
		_tftCmd(0xC1);
		_tftData(0x0010);
		_tftData(0x0010);
		_tftData(0x0002);
		_tftData(0x0002);
		_tftCmd(0xC0);		//Set Default Gamma
		_tftData(0x0000);
		_tftData(0x0035);
		_tftData(0x0000);
		_tftData(0x0000);
		_tftData(0x0001);
		_tftData(0x0002);
		_tftCmd(0xC5);		//Set frame rate
		_tftData(0x0004);
		_tftCmd(0xD2);		//power setting
		_tftData(0x0001);
		_tftData(0x0044);
		_tftCmd(0xC8);		//Set Gamma
		_tftData(0x0004);
		_tftData(0x0067);
		_tftData(0x0035);
		_tftData(0x0004);
		_tftData(0x0008);
		_tftData(0x0006);
		_tftData(0x0024);
		_tftData(0x0001);
		_tftData(0x0037);
		_tftData(0x0040);
		_tftData(0x0003);
		_tftData(0x0010);
		_tftData(0x0008);
		_tftData(0x0080);
		_tftData(0x0000);
		_tftCmd(0x2A);		//Set column address (2A)
		_tftData(0x0000);
		_tftData(0x0020);
		_tftData(0x0001);
		_tftData(0x00AF);
		_tftCmd(0x2B);		//Set page address (2B)
		_tftData(0x0000);
		_tftData(0x0000);
		_tftData(0x0000);
		_tftData(0x00EF);
		_tftCmd(0x29);		//display on      
		_tftCmd(0x2C);		//display on
		SET_CS;
	#endif
	fdevopen(ATputchar, NULL );	
	tftClear(BLACK);

}
void _tftSetXY(unsigned int x,unsigned char y)
{
	#ifdef CONTROLLER_SSD1289
		#define _tftSetXYQM(x,y){ \
	  	_tftCmdData(0x4e,y); \
		_tftCmdData(0x4f,319-x);\
		_tftCmd(0x0022); }
	#endif
	#ifdef CONTROLLER_ILI9327
	 	#define _tftSetXYQM(x,y){ \
	  	_tftCmd(0x2A); \
		_tftData((0x20+x)>>8);\
		_tftData(0x20+x);\
		_tftCmd(0x2B);\
		_tftData(0);\
		_tftData(y);\
		_tftCmd(0x2C); }
	#endif
   _tftSetXYQM(x,y);
}

BOOL regionReseted=0;
void _tftResetRegion()
{
	#ifdef CONTROLLER_SSD1289
		//Landscape
		_tftCmdData(0x44,239<<8);
		_tftCmdData(0x45,0);
		_tftCmdData(0x46,319);
	#endif
	#ifdef CONTROLLER_ILI9327
		_tftCmd(0x2A);		//Set column address (2A)
		_tftData(0x0000);
		_tftData(0x0020);
		_tftData(0x0001);
		_tftData(0x00AF);
		_tftCmd(0x2B);		//Set page address (2B)
		_tftData(0x0000);
		_tftData(0x0000);
		_tftData(0x0000);
		_tftData(0x00EF);
	#endif	 
	regionReseted=1;
}	 
void _tftSetRegionAndPos(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2)
{
	#ifdef CONTROLLER_SSD1289
		//Landscape
		_tftCmdData(0x44,(y2<<8)+y1);
		_tftCmdData(0x45,319-x2);
		_tftCmdData(0x46,319-x1);
		_tftSetXYQM(x1,y1);	
	#endif
	#ifdef CONTROLLER_ILI9327
		x1+=0x20;
		x2+=0x20;	
		_tftCmd(0x2A);		//Set column address (2A)
		_tftData(x1>>8);
		_tftData(x1);
		_tftData(x2>>8);
		_tftData(x2);
		_tftCmd(0x2B);		//Set page address (2B)
		_tftData(y1>>8);
		_tftData(y1);
		_tftData(y2>>8);
		_tftData(y2);    
		_tftCmd(0x2C);		//start writing
	#endif 
	regionReseted=0;
}
void tftDrawPixel(unsigned int x,unsigned char y,unsigned int color)
{
	CLEAR_CS;
	if(!regionReseted)
	 _tftResetRegion();
	_tftSetXYQM(x,y);
	_tftDataQM(color);
	SET_CS;
}
void tftClear(unsigned int color)
{
	unsigned long i;
	CLEAR_CS;
	if(!regionReseted)
		_tftResetRegion();
	_tftSetXY(0,0);
	for(i=(WIDTH*HEIGHT); i>0; i--)	_tftDataQM(color);
	 SET_CS;
}
void tftDrawLine(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color)
{
	CLEAR_CS;
   	if(!regionReseted)
   		_tftResetRegion();
	if(y1==y2)
	{
		if(x2<x1)
		{
			_tftSetXY(x2,y1);
			do _tftDataQM(color)	    //Let Semi away, because of block makro
			while(x2++!=x1);
		}
		else
		{
			_tftSetXY(x1,y1);
			do _tftDataQM(color)
			while(x1++!=x2);
		}		
	}
	else if(x1==x2)
	{
		   #ifdef CONTROLLER_ILI9327
		   _tftSetRegionAndPos(x1,((y1>y2)?y2:y1),x1,((y1>y2)?y1:y2));//_tftCmdData(0x36,0x0049);  //Set_address_mode (36)
		  #else
		   _tftCmdData(0x11,0x6030); // Change adresspointer direction
		  #endif
		  
		  // (is quicker than defining a region)
		  if(y2<y1)
		  {
			   _tftSetXY(x1,y2);
			   do _tftDataQM(color)
			   while(y2++!=y1);
		  }
		  else
		  {
			   _tftSetXY(x1,y1);
			   do _tftDataQM(color) 
			   while(y1++!=y2);
		  }
		  #ifdef CONTROLLER_ILI9327
		   _tftResetRegion();  //_tftCmdData(0x36,0x0069);  //Set_address_mode (36)
		  #else
		   _tftCmdData(0x11,0x6018); // Set adresspointer direction normal again
		  #endif
	}
	else
	{
		if(abs(x2-x1) > abs(y2-y1))
		{	
			//Without floating point!
			int deltax = ((int)x2-(int)x1);
			int deltay = ((int)y2-(int)y1)<<1;	// multiple by 2 to make it easier to round
			int x = 0;
			if (x1>x2)
			{
				do
				{ 
					//y = mx + b (math theory, linear functions)
					_tftSetXY(x1+x,y1+ (((long)deltay*(long)x/deltax+1)>>1)); // Add 1 and divde by 2 = +0.5
					_tftDataQM(color);
				}
				while(x--!=deltax);
			}
			else
			{
				do
				{ 
					_tftSetXY(x1+x,y1+ (((long)deltay*(long)x/deltax+1)>>1));
					_tftDataQM(color);
				}
				while(x++!=deltax);
			}
		}
		else
		{
			int deltax = ((int)x2-(int)x1)<<1;
			int deltay = ((int)y2-(int)y1);
			int y = 0;
			if (y1>y2)
			{
				do
				{ 
					_tftSetXY(x1+ (((long)deltax*(long)y/deltay+1)>>1),y1+ y);
					_tftDataQM(color);
				}
				while(y--!=deltay);
			}
			else
			{
				do
				{ 
					_tftSetXY(x1+ (((long)deltax*(long)y/deltay+1)>>1),y1+ y);
					_tftDataQM(color);
				}
				while(y++!=deltay);
			} 	
		} 
	}	 
	SET_CS;
}

void tftDrawRectangle(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color)
{
	unsigned int tmp;
	unsigned int i;
	CLEAR_CS;
   	if(!regionReseted)
		_tftResetRegion();
	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	i=x1;
	_tftSetXY(x1,y1);
	while(i++!=x2) _tftDataQM(color);
	_tftSetXY(x1,y2);
	while(i--!=x1) _tftDataQM(color);
	i=y1;
	#ifdef CONTROLLER_SSD1289
	_tftCmdData(0x11,0x6030); // Change adresspointer direction
	_tftSetXY(x1,y1);
	#else
	_tftSetRegionAndPos(x1,y1,x1,y2);
	#endif
	while(i++!=y2) _tftDataQM(color);
	#ifdef CONTROLLER_SSD1289
	_tftSetXY(x2,y1);
	#else
	_tftSetRegionAndPos(x2,y1,x2,y2);
	#endif
	while(i--!=y1) _tftDataQM(color);
	#ifdef CONTROLLER_SSD1289
	_tftCmdData(0x11,0x6018); // Set adresspointer direction normal again
	#else
	_tftResetRegion(); 
	#endif
	SET_CS;
}
void tftFillRectangle(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color)
{
	unsigned int tmp;
	unsigned long i;
	CLEAR_CS;
	if (x1>x2)
	{
		tmp=x1;
		x1=x2;
		x2=tmp;
	}
	if (y1>y2)
	{
		tmp=y1;
		y1=y2;
		y2=tmp;
	}
	_tftSetRegionAndPos(x1,y1,x2,y2);
	for(i=((unsigned long)(x2-x1+1)*(y2-y1+1));i>0;i--) _tftDataQM(color);
  	SET_CS;
}

void tftDrawBitmapUnscaled(unsigned int x, unsigned char y, unsigned int width, unsigned char height, FLASH_INT_ARR_POINTER dat)
{
   	unsigned long i;
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+height-1);
	for(i=height*width; i>0; i--)
	{
		_tftDataQM(FLASH_INT_ARR_READ(dat));
		dat++;
	}
	SET_CS;
}


void tftDrawBitmapUnscaledStreamedRaw (unsigned int x, unsigned char y, unsigned int width, unsigned char height, char* filename)
{
	WORD readed;
	if(pf_open(filename)!=FR_OK) return;
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+height-1);
	SET_RS;
	while (1)
	{
		pf_read(NULL,65024,&readed);
		if (readed < 65024)
			break;
	}
	SET_CS;
}

void _tftPutChar(unsigned int x, unsigned char y, unsigned char c, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font)
{
	unsigned char width = FLASH_CHAR_ARR_READ(&font[0]);
	unsigned char height = FLASH_CHAR_ARR_READ(&font[1]);
	unsigned char offset = FLASH_CHAR_ARR_READ(&font[2]);
	switch(c)
	{
		case 0xC4: //Ä
			c=127;
		break;
		case 0xD6: //Ö
			c=128;
		break;
		case 0xDC: //Ü
			c=129;
		break;
		case 0xE4: //ä
			c=130;
		break;
		case 0xF6: //ö
			c=131;
		break;
		case 0xFC: //ü
			c=132;
		break;
		default:
		break;
	}
	unsigned int ind = ((c-offset)*((width/8)*height))+4;
	unsigned int cnt =0;
	unsigned char bitm=0;
	BOOL bgIsTrans = bgcolor==TRANSPARENT;
	#ifdef CONTROLLER_SSD1289
	BOOL enTrans=0;
	#endif
	#ifdef CONTROLLER_ILI9327
	unsigned char xadd=0;
	unsigned char yadd=0;
	#endif
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+height-1);
	for(cnt= (width/8)*height; cnt>0; cnt--)
	{
		for(bitm=0x80; bitm >0; bitm>>=1)
		{
			if(FLASH_CHAR_ARR_READ(&font[ind])& bitm)
			{
				#ifdef CONTROLLER_SSD1289
					if(enTrans)
					{
						enTrans=0;
						_tftCmdData(0x23,0x0000); 
						_tftCmdData(0x24,0x0000); 
						_tftCmd(0x22);
					}
					_tftDataQM(color)
				#endif
				#ifdef CONTROLLER_ILI9327
					if(bgIsTrans) 
						_tftSetXY(x+xadd,y+yadd);
					_tftDataQM(color);
				#endif
			}
			else
			{
				#ifdef CONTROLLER_SSD1289
					if(bgIsTrans && !enTrans)
					{
						enTrans=1;
						_tftCmdData(0x23,0xFFFF); 
						_tftCmdData(0x24,0xFFFF); 
						_tftCmd(0x22);
					}
					_tftDataQM(bgcolor)	
				#endif
				#ifdef CONTROLLER_ILI9327
					if(!bgIsTrans) _tftDataQM(bgcolor);
				#endif
				
			}
			#ifdef CONTROLLER_ILI9327
			if(bgIsTrans) 
			{
				xadd++;
				if(xadd==width)
				{
					xadd=0;
					yadd++;
				}
			}
			#endif
		}
		ind++;
	}
	#ifdef CONTROLLER_SSD1289
		if(enTrans)
		{
			_tftCmdData(0x23,0x0000);
			_tftCmdData(0x24,0x0000); 
		}
	#endif
	SET_CS;
}

unsigned char fontWidth=0;
unsigned int textX=0;
unsigned char textY=0;
unsigned int textColor=0;
unsigned int textBgcolor=0;
FLASH_CHAR_ARR_POINTER fontPtr;
unsigned int tftPrintCalcWidth(FLASH_CHAR_ARR_POINTER font,  char * text)
{
	return FLASH_CHAR_ARR_READ(&font[0]) * strlen(text);
}/*
unsigned int tftPrintCalcWidth_P(FLASH_CHAR_ARR_POINTER font,  const char * text)
{
	return FLASH_CHAR_ARR_READ(&font[0]) * strlen_P(text);
}*/

unsigned char tftPrintCalcHeight(FLASH_CHAR_ARR_POINTER font)
{
	return FLASH_CHAR_ARR_READ(&font[1]);
}

void tftPrint(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font,  char * text)
{
	fontWidth = FLASH_CHAR_ARR_READ(&font[0]);
	char c = *text;
	while(c!=0x00)
	{
		 _tftPutChar(x, y, c, color,  bgcolor,  font);
		 x += fontWidth;
		 text++;
		 c = *text;
	}
}

void tftPrint_P(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font,  const char * text)
{
	fontWidth = FLASH_CHAR_ARR_READ(&font[0]);
	char c = FLASH_CHAR_ARR_READ(text);
	while(c!=0x00)
	{
		 _tftPutChar(x, y, c, color,  bgcolor,  font);
		 x += fontWidth;
		 text++;
		 c = FLASH_CHAR_ARR_READ(text);
	}
}

void tftPrintf(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font, char *format, ...)
{
	va_list args;
	fontPtr =font;
	fontWidth = FLASH_CHAR_ARR_READ(&font[0]);
	textX = x;
	textY = y;
	textColor = color;
	textBgcolor=bgcolor;
	 
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
}
void tftPrintf_P(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font,const char *format, ...)
{
	va_list args;
	fontPtr =font;
	fontWidth = FLASH_CHAR_ARR_READ(&font[0]);
	textX = x;
	textY = y;
	textColor = color;
	textBgcolor=bgcolor;
	 
	va_start (args, format);
	vfprintf_P (stdout,format, args);
	va_end (args);
}


int ATputchar(char c,FILE*f)
{
 if(fontWidth>WIDTH)
  return EOF;
 _tftPutChar(textX, textY, c,  textColor,  textBgcolor,  fontPtr);
 textX += fontWidth;
  return 0;
}
//Draw an icon, for example e battery percentage icon
void tftDrawIconUnscaledStreamedRaw(unsigned int x, unsigned char y, unsigned char width, unsigned char iconNr,  char* filename)
{
	#define ICONS_PER_ROW	10
	WORD readed;
	if(pf_open(filename)!=FR_OK) return;
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+width-1);
	SET_RS;
	DWORD SDoffset = 2*(iconNr%ICONS_PER_ROW)*width+(DWORD)(iconNr/ICONS_PER_ROW)*ICONS_PER_ROW*2*width*width;
	for(unsigned char i=0; i<width; i++)
	{
		pf_lseek(SDoffset);
		pf_read(NULL,2*width,&readed);		//read (width) Pixel
		SDoffset += width*2*ICONS_PER_ROW;
	}
	SET_CS;
}

void tftDrawGradient(unsigned int x, unsigned char y, unsigned int width, unsigned char height, FLASH_INT_ARR_POINTER dat)
{
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+height-1);
	unsigned int w_cnt=0;
	while(height>0)
	{	
		_tftDataQM(FLASH_INT_ARR_READ(dat));
		if (++w_cnt==width)
		{
			dat++;
			height--;
			w_cnt=0;
		}
	}	
	SET_CS;
}
void tftDrawLinearGradient(unsigned int x, unsigned char y, unsigned int width, unsigned char height, unsigned int color_top, unsigned int color_bot,GRADIENT_MODE mode)
{
	CLEAR_CS;
	_tftSetRegionAndPos(x,y,x+width-1,y+height-1);
	int h=0;
	int w=0;
	unsigned int w_cnt=0;
	unsigned int col=0;
	int r,g,b;
	int dr,dg,db;
	int ar,ag,ab;
	r=(color_top&0xF800)>>11;
	g=(color_top&0x07E0)>>5;
	b=(color_top&0x001F)>>0;
	dr=((color_bot&0xF800)>>11)-r;
	dg=((color_bot&0x07E0)>>5)-g;
	db=((color_bot&0x001F)>>0)-b;
	if(mode==TOP_BOTTOM)
	{
		while(h<height)
		{
			//TODO Remove unuseful int cast and variables
			ar= r+((dr*h)+((int)height/2))/((int)height);
			ag = g+((dg*h)+((int)height/2))/((int)height);
			ab = b+((db*h)+((int)height/2))/((int)height);

			col = (ar<<11)|(ag<<5)|(ab&0x1F);
			while(w_cnt++<width) _tftDataQM(col);			
			w_cnt=0;
			h++;
		
		}	
	}
	else if(mode==LEFT_RIGHT)
	{
		while(h++<height)
		{
			while(w<width)
			{
				//TODO Remove unuseful int cast and variables
				ar= r+((dr*w)+((int)width/2))/((int)width);
				ag = g+((dg*w)+((int)width/2))/((int)width);
				ab = b+((db*w)+((int)width/2))/((int)width);
				col = (ar<<11)|(ag<<5)|(ab&0x1F);
				_tftDataQM(col);			
				w++;
			}	
			w=0;
		}
		
	}
	
	SET_CS;
}





void tftSetScrollArea(unsigned int TFA, unsigned int VSA, unsigned int BFA )
//ILI9327 Manual Page 81
{
	CLEAR_CS;
	#ifdef CONTROLLER_SSD1289
	
	#endif
	#ifdef CONTROLLER_ILI9327
		//_tftCmd(0x36);		//Set_address_mode (36)
		//_tftData(0x0009);
		_tftCmd(0x33);
		_tftData(TFA>>8);
		_tftData(TFA&0x00FF);
		_tftData(VSA>>8);
		_tftData(VSA&0x00FF);
		_tftData(BFA>>8);
		_tftData(BFA&0x00FF);
	#endif
	SET_CS;
}
void tftSetScrollStart(unsigned int VSP)
//ILI9327 Manual Page 89
{
	CLEAR_CS;
	#ifdef CONTROLLER_SSD1289
	#endif
	#ifdef CONTROLLER_ILI9327
		_tftCmd(0x37);
		_tftData(VSP>>8);
		_tftData(VSP&0x00FF);
	#endif
	SET_CS;
}
void tftEnterSleep()
{
	CLEAR_CS;
	_tftCmd(0x10);		//Enter sleep Mode 
	SET_CS;
}
void tftExitSleep()
{
	CLEAR_CS;
	_tftCmd(0x11);		//Exit sleep Mode 
	SET_CS;
}