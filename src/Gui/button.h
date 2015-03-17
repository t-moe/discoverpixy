
typedef void (*BUTTON_CALLBACK)(void *button);	//!< Function pointer used...
typedef struct {
	TOUCH_AREA_STRUCT base; 
	unsigned int bgcolor;
	BUTTON_CALLBACK callback; //Callback
	unsigned int txtcolor;
	FLASH_CHAR_ARR_POINTER font;
	char *text;
	
} BUTTON_STRUCT;

typedef struct {
	TOUCH_AREA_STRUCT base;
	unsigned int bgcolor;
	BUTTON_CALLBACK callback; //Callback
	unsigned char imgwidth;
	unsigned char imgheight;
	char* filename;
} BITMAPBUTTON_STRUCT;

//Notice that the first 3 Members are Equal, so it's possible to cast it to a BUTTON_STRUCT even if it's a BITMAPBUTTON_STRUCT (when changeing only the first 3 Members).

#define AUTO 0

extern BOOL guiAddButton(BUTTON_STRUCT* button);
extern void guiRemoveButton(BUTTON_STRUCT* button);
extern void guiRedrawButton(BUTTON_STRUCT* button);

extern BOOL guiAddBitmapButton(BITMAPBUTTON_STRUCT* button);
extern void guiRemoveBitmapButton(BITMAPBUTTON_STRUCT* button);
extern void guiRedrawBitmapButton(BITMAPBUTTON_STRUCT* button);
