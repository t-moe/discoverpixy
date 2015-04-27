
typedef void (*CHECKBOX_CALLBACK)(void *checkbox, bool checked);	//!< Function pointer used...
typedef struct {
	TOUCH_AREA_STRUCT base; 
	uint16_t fgcolor;
	bool checked;
	CHECKBOX_CALLBACK callback; //Callback
} CHECKBOX_STRUCT;


bool gui_checkbox_add(CHECKBOX_STRUCT* checkbox);
void gui_checkbox_remove(CHECKBOX_STRUCT* checkbox);
void gui_checkbox_update(CHECKBOX_STRUCT* checkbox);
void gui_checkbox_redraw(CHECKBOX_STRUCT* checkbox);
#define CHECKBOX_WIN_FG_COLOR RGB(32,161,34)
