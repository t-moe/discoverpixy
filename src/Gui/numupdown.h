//
//
// SUPPORT DISCONTINUED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
//
typedef void (*NUMUPDOWN_CALLBACK)(void *numupdown, unsigned char value);	//!< Function pointer used...
typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned int fgcolor;
	unsigned char value;
	unsigned char min;
	unsigned char max;
	NUMUPDOWN_CALLBACK callback; //Callback
	BUTTON_STRUCT buttonUp;
	BUTTON_STRUCT buttonDown;
} NUMUPDOWN_STRUCT;


extern BOOL guiAddNumUpDown(NUMUPDOWN_STRUCT* numupdown);
extern void guiRemoveNumUpDown(NUMUPDOWN_STRUCT* numupdown);
extern void guiUpdateNumUpDown(NUMUPDOWN_STRUCT* numupdown);
extern void guiRedrawNumUpDown(NUMUPDOWN_STRUCT* numupdown);