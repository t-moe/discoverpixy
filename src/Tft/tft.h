


extern void tftInit(void);
extern void tftClear(unsigned int color);
extern void tftDrawPixel(unsigned int x,unsigned char y,unsigned int color);
extern void tftDrawLine(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color);
extern void tftDrawRectangle(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color);
extern void tftFillRectangle(unsigned int x1,unsigned char y1,unsigned int x2,unsigned char y2, unsigned int color);
extern void tftDrawBitmapUnscaled(unsigned int x, unsigned char y, unsigned int width, unsigned char height, FLASH_INT_ARR_POINTER dat);

extern void tftPrint(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font,  char * text);
extern void tftPrint_P(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor,  FLASH_CHAR_ARR_POINTER font, const char * text);
extern void tftPrintf(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font, char *format, ...);
extern void tftPrintf_P(unsigned int x, unsigned char y, unsigned int color, unsigned int bgcolor, FLASH_CHAR_ARR_POINTER font,const char *format, ...);
extern void tftDrawBitmapUnscaledStreamedRaw (unsigned int x, unsigned char y, unsigned int width, unsigned char height,  char* filename);
extern void tftDrawIconUnscaledStreamedRaw(unsigned int x, unsigned char y, unsigned char width, unsigned char iconNr, char* filename);
typedef enum {TOP_BOTTOM,LEFT_RIGHT} GRADIENT_MODE;
extern void tftDrawLinearGradient(unsigned int x, unsigned char y, unsigned int width, unsigned char height, unsigned int color_top, unsigned int color_bot, GRADIENT_MODE mode);
extern void tftDrawGradient(unsigned int x, unsigned char y, unsigned int width, unsigned char height, FLASH_INT_ARR_POINTER dat);
extern unsigned int tftPrintCalcWidth(FLASH_CHAR_ARR_POINTER font,  char * text);
extern unsigned int tftPrintCalcWidth_P(FLASH_CHAR_ARR_POINTER font,  const char * text);
extern unsigned char tftPrintCalcHeight(FLASH_CHAR_ARR_POINTER font);
extern void tftEnterSleep();
extern void tftExitSleep();

#define RGB(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))
#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define WHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)
#define HEX(h) (RGB(((h)>>16),((h)>>8),(h)))
#define TRANSPARENT 0x80C2

#ifdef CONTROLLER_SSD1289
	#define WIDTH 320l
	#define HEIGHT 240l
#elif defined (CONTROLLER_ILI9327)
	#define WIDTH 400l
	#define HEIGHT 240l
#else
	  #error You should either define CONTROLLER_SSD1289 or CONTROLLER_ILI9327 in the Compiler Settings
#endif
#define DWIDTH (WIDTH-1)
#define DHEIGHT (HEIGHT-1)
#define HWIDTH (WIDTH/2)
#define HHEIGHT (HEIGHT/2)

extern FLASH_CHAR_ARR(SmallFont,1144);
extern FLASH_CHAR_ARR(BigFont,3044);
extern FLASH_CHAR_ARR(SevenSegNumFont,2004);