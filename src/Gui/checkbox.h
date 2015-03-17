
typedef void (*CHECKBOX_CALLBACK)(void *checkbox, BOOL checked);	//!< Function pointer used...
typedef struct {
	TOUCH_AREA_STRUCT base; 
	unsigned int fgcolor;
	BOOL checked;
	CHECKBOX_CALLBACK callback; //Callback
} CHECKBOX_STRUCT;


extern BOOL guiAddCheckbox(CHECKBOX_STRUCT* checkbox);
extern void guiRemoveCheckbox(CHECKBOX_STRUCT* checkbox);
extern void guiUpdateCheckbox(CHECKBOX_STRUCT* checkbox);
extern void guiRedrawCheckbox(CHECKBOX_STRUCT* checkbox);
#define CHECKBOX_WIN_FG_COLOR RGB(32,161,34)