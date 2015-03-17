typedef enum {NONE=0x00,PEN_DOWN=0x01, PEN_UP=0x02, PEN_ENTER=0x04, PEN_LEAVE=0x08,PEN_MOVE=0x10} TOUCH_ACTION;


typedef void (*TOUCH_CALLBACK)(void* touchArea, TOUCH_ACTION triggeredAction);	


typedef struct {
	TOUCH_ACTION hookedActions; //Actions to listen to
	unsigned int x1; //Top Left X Coordiate of Area
	unsigned int y1; //Top Left Y Coordiate of Area
	unsigned int x2; //Bottom Right X Coordiate of Area
	unsigned int y2; //Bottom Right Y Coordiate of Area
	TOUCH_CALLBACK callback; //Callback
	UCHAR flags; //Internal Used, don't change
} TOUCH_AREA_STRUCT;


extern volatile int touchY;
extern volatile int touchX;

extern void touchInit();
extern void touchCalibrate();
extern BOOL touchHaveEmpty(unsigned char num);
extern BOOL touchRegisterArea(TOUCH_AREA_STRUCT* area);
extern void touchUnregisterArea(TOUCH_AREA_STRUCT* area);


