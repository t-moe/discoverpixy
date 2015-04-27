
typedef void (*BUTTON_CALLBACK)(void *button);	//!< Function pointer used...
typedef struct {
	TOUCH_AREA_STRUCT base; 
	uint16_t bgcolor;
	BUTTON_CALLBACK callback; //Callback
	uint16_t txtcolor;
	uint8_t font;
	const char *text;
	
} BUTTON_STRUCT;
/*
typedef struct {
	TOUCH_AREA_STRUCT base;
	unsigned int bgcolor;
	BUTTON_CALLBACK callback; //Callback
	unsigned char imgwidth;
	unsigned char imgheight;
	char* filename;
} BITMAPBUTTON_STRUCT;
*/
//Notice that the first 3 Members are Equal, so it's possible to cast it to a BUTTON_STRUCT even if it's a BITMAPBUTTON_STRUCT (when changeing only the first 3 Members).

#define AUTO 0

bool gui_button_add(BUTTON_STRUCT* button);
void gui_button_remove(BUTTON_STRUCT* button);
void gui_button_redraw(BUTTON_STRUCT* button);

/*
bool guiAddBitmapButton(BITMAPBUTTON_STRUCT* button);
void guiRemoveBitmapButton(BITMAPBUTTON_STRUCT* button);
void guiRedrawBitmapButton(BITMAPBUTTON_STRUCT* button);

*/
