#ifndef TOUCH_H
#define TOUCH_H


#include<stdbool.h>
#include<stdint.h>


typedef enum {TOUCH_UP,TOUCH_DOWN} TOUCH_STATE ;
typedef enum {NONE=0x00,PEN_DOWN=0x01, PEN_UP=0x02, PEN_ENTER=0x04, PEN_LEAVE=0x08,PEN_MOVE=0x10} TOUCH_ACTION;

typedef void (*TOUCH_CALLBACK)(void* touchArea, TOUCH_ACTION triggeredAction);

typedef struct {
        TOUCH_ACTION hookedActions; //Actions to listen to
        uint16_t x1; //Top Left X Coordiate of Area
        uint16_t y1; //Top Left Y Coordiate of Area
        uint16_t x2; //Bottom Right X Coordiate of Area
        uint16_t y2; //Bottom Right Y Coordiate of Area
        TOUCH_CALLBACK callback; //Callback
        uint8_t flags; //Internal Used, don't change
} TOUCH_AREA_STRUCT;


typedef struct {
	uint16_t x;
	uint16_t y;
} POINT_STRUCT;


bool touch_init();
bool touch_add_raw_event(uint16_t x, uint16_t y,TOUCH_STATE state);
bool touch_have_empty(unsigned char num);
bool touch_register_area(TOUCH_AREA_STRUCT* area);
void touch_unregister_area(TOUCH_AREA_STRUCT* area);
POINT_STRUCT touch_get_last_point();


#endif /* TOUCH_H */
