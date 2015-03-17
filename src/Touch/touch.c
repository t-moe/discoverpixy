
#include "../settings.h"
#include"../types.h"
#include "../OpSys/ptask.h"
#include "touch.h"
#include "../Tft/tft.h"
#include <string.h>

#define CCENTER 20  //Pixel Distance from Sides for Calibration Cross
#define CLENGTH 10	//Length of the Calibration Cross Lines
#define TOUCH_PRECISION	16 //Number of executions before building the average
#define NUM_AREAS 50	//Number of Structs Reserved in Memory for TouchAreas (e.g Buttons) 
#define TIMER_FREQ 15 //Frequency of the Interval between the finsihed and the next measure, while pen is down.
#define TOUCH_BASEPRIORITY 100


//Global
volatile int touchY=0; //Last Y Coordinate in pixels
volatile int touchX=0; //Last X Coordinate in pixels

volatile BOOL calibration =0 ; //Flag if calibartion active (values will not be converted to pixels)
unsigned char touchEvents[28]; //7 bytes per Event (1 Length + 6 Data)
PTASK_STRUCT* touchTask;
TOUCH_AREA_STRUCT* areas[NUM_AREAS] = {NULL};
#ifdef CONTROLLER_SSD1289
int _x1=0x0231;
int _x2=0x0C08;
int _y1=0x0287;
int _y2=0x0B56;  //Calibration Constants(Defaults= Timo's 3.2")
#endif
#ifdef CONTROLLER_ILI9327
int _x1=0x0DEE;
int _x2=0xF34E;
int _y1=0x01EF;
int _y2=0x0BE2;  //Calibration Constants(Defaults= Timo's 3.2")

#endif

#define dx _x2 //Save some vars
#define dy _y2 
#define CBEGIN (CCENTER-CLENGTH/2)
#define CEND (CCENTER + CLENGTH/2)



void touchTaskCb(void *this_, UCHAR length)
{
	//Because we have Data, we need to read the event, or the Function will be called ever and ever again.
	unsigned char dat[6]; // Parameter length will be 6, i know it :)
	PTask_ReadEvent(this_,dat); //Read current Event
	unsigned char penDown = dat[0];
	unsigned char oldPenDown = dat[1];
	unsigned int resX = dat[2]<<8 | dat[3];
	unsigned int resY = dat[4]<<8 | dat[5];
	unsigned char z;
	if(penDown)
	{
		touchX=(((long)(DWIDTH-2*CCENTER)*2*(long)((long)resX-_x1)/dx+1)>>1)+CCENTER;  
		touchY=(((long)(DHEIGHT-2*CCENTER)*2*(long)((long)resY-_y1)/dy+1)>>1)+CCENTER;
	//	tftDrawPixel(touchX,touchY,WHITE);
		if(!oldPenDown)	//First Touch
		{
			for(z=0; z < NUM_AREAS; z++) // For every touch area
			{
				if(areas[z]!=NULL  && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
				{
					areas[z]->flags=1; //PenInside=1
					if(areas[z]->hookedActions & PEN_DOWN)
						areas[z]->callback(areas[z],PEN_DOWN);
				}				
			}		
		}	
		else //Second, Third
		{
			for(z=0; z < NUM_AREAS; z++) // For every touch area
			{
				if(areas[z]!=NULL )
				{
					if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
					{
						if(areas[z]->flags==0)  //PenInside ==0
						{
							areas[z]->flags=1; //PenInside=1
							if(areas[z]->hookedActions & PEN_ENTER)
								areas[z]->callback(areas[z],PEN_ENTER);
						}
					}
					else if(areas[z]->flags) //PenInside==1
					{
						areas[z]->flags=0; //PenInside=0
						if(areas[z]->hookedActions & PEN_LEAVE)
							areas[z]->callback(areas[z],PEN_LEAVE);
					}
				}
			}			
		}
		for(z=0; z < NUM_AREAS; z++) // For every touch area
		{
			if(areas[z]!=NULL && areas[z]->hookedActions&PEN_MOVE)
			{
				if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
				{
					areas[z]->callback(areas[z],PEN_MOVE);
				}
			}
		}											
	}
	else
	{
		if(oldPenDown) //Was the pen ever down (or was it a too short touch)
		{
			for(z=0; z < NUM_AREAS; z++) // For every touch area
			{
				if(areas[z]!=NULL && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
				{
					areas[z]->flags=0; //PenInside = 0;
					if(areas[z]->hookedActions & PEN_UP)
						areas[z]->callback(areas[z],PEN_UP);
				}				
			}
		}
		touchX=0xFFFF;
		touchY=0xFFFF;
	}
	
}
void touchInit (void)
{
		//SPI D1 Touch
		USARTD1.BAUDCTRLA = 255;
		USARTD1.BAUDCTRLB = 0;
		USARTD1.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;		//Receive Enable, Transmit Enable
		USARTD1.CTRLC |= USART_CMODE_MSPI_gc;				//Master SPI Mode
		USARTD1.CTRLA = USART_RXCINTLVL_LO_gc;				//Interrupt Level0 Enabled

		
		PORTD.DIRSET = Touch_SPI_SCK_bm | Touch_SPI_DOUT_bm;	//SCK und TX
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;		//RX
		PORTE.DIRSET = Touch_SPI_CS_bm;
		PORTE.OUTSET = Touch_SPI_CS_bm;				//Set CS
		
		USARTD1.CTRLC &= ~(0x02);		//UCPHA bit löschen
		
		//Touch Interrupt
		PORTD.DIRCLR = Touch_Interrupt_bm;
		PORTD.INT0MASK = Touch_Interrupt_bm;
		PORTD.PIN4CTRL |= PORT_OPC_PULLUP_gc;
		PORTD.PIN4CTRL |= PORT_ISC_FALLING_gc;
		PORTD.INTCTRL = PORT_INT0LVL_LO_gc;
		
		TCD1.PER = (F_CPU / 256 / TIMER_FREQ) -1; 
		TCD1.INTCTRLA = TC_OVFINTLVL_LO_gc;
		
		touchTask = PTask_AddTask(touchTaskCb,TOUCH_BASEPRIORITY,28,touchEvents);
	
	
		//HACK this is just a bad Workaround, to prevent touch Problems after start
		//you can also use: TCD1.CTRLA = TC_CLKSEL_DIV256_gc; 
		CLEAR_CS_TOUCH;
		USARTD1.DATA = 0x90;
}

ISR(TCD1_OVF_vect)
{
	TCD1.CTRLA = TC_CLKSEL_OFF_gc;
	//TCD1.CNT = 0x00;
	CLEAR_CS_TOUCH;
	USARTD1.DATA = 0x90;
}

ISR (PORTD_INT0_vect)
{
	PORTD.INTCTRL = PORT_INT0LVL_OFF_gc;	//Deactivate INT0ISR
	TCD1.CNT = 0x00;
	TCD1.CTRLA = TC_CLKSEL_DIV256_gc;  //Activate Timer
}
void touchCalibrate()
{
	#define TOUCH_DEBUG //Define if Debug Values should be printed
	tftClear(BLACK);
	tftPrint_P(HWIDTH-27*8/2, HHEIGHT-6, WHITE, BLACK, SmallFont, PSTR("Tippe genau auf den Marker!"));
	//-----------------First Point--------------------
	tftDrawLine(CCENTER,CBEGIN,CCENTER,CEND,WHITE);	//Draw Cross
	tftDrawLine(CBEGIN,CCENTER,CEND,CCENTER,WHITE);	//Draw Cross
    calibration=1; //TouchX + TouchY Values will not be converted to Pixels
	while(calibration);	//Wait on PenUp
	_x1=touchX;
	_y1=touchY;
	#ifdef TOUCH_DEBUG
		tftPrintf_P(CEND+5,CEND+5,WHITE,BLUE, SmallFont, PSTR("X:0x%03X Y:0x%03X"),touchX,touchY);
	#endif
	tftFillRectangle(CBEGIN,CBEGIN,CEND,CEND,BLACK); //Clear Cross
	//-----------------Second Point-------------------
	tftDrawLine(DWIDTH-CCENTER,DHEIGHT-CBEGIN,DWIDTH-CCENTER,DHEIGHT-CEND,WHITE);
	tftDrawLine(DWIDTH-CBEGIN,DHEIGHT-CCENTER,DWIDTH-CEND,DHEIGHT-CCENTER,WHITE);
	calibration=1;
	while(calibration);
	_x2=touchX;
	_y2=touchY;
	#ifdef TOUCH_DEBUG
		tftPrintf_P(DWIDTH-CEND-17*8,DHEIGHT-CEND-5-12,WHITE,BLUE, SmallFont, PSTR("X:0x%03X Y:0x%03X"),touchX,touchY);
	#endif
	tftFillRectangle(DWIDTH-CBEGIN,DHEIGHT-CBEGIN,DWIDTH-CEND,DHEIGHT-CEND,BLACK);
	//-----------------Third Point--------------------
	tftDrawLine(CCENTER,DHEIGHT-CBEGIN,CCENTER,DHEIGHT-CEND,WHITE);
	tftDrawLine(CBEGIN,DHEIGHT-CCENTER,CEND,DHEIGHT-CCENTER,WHITE);
 	calibration=1;
	while(calibration);
	_x1+=touchX; //Add(!) values. We'll build the average later
	_y2+=touchY;
	#ifdef TOUCH_DEBUG
		tftPrintf_P(CEND+5,DHEIGHT-CEND-5-12,WHITE,BLUE, SmallFont, PSTR("X:0x%03X Y:0x%03X"),touchX,touchY);
	#endif
	tftFillRectangle(CBEGIN,DHEIGHT-CBEGIN,CEND,DHEIGHT-CEND,BLACK);
	//------------------4. Point---------------------
	tftDrawLine(DWIDTH-CCENTER,CBEGIN,DWIDTH-CCENTER,CEND,WHITE);
	tftDrawLine(DWIDTH-CBEGIN,CCENTER,DWIDTH-CEND,CCENTER,WHITE);
	calibration=1;
	while(calibration);
	_x2+=touchX;
	_y1+=touchY;
	#ifdef TOUCH_DEBUG
		tftPrintf_P(DWIDTH-CEND-17*8,CEND+5,WHITE,BLUE, SmallFont, PSTR("X:0x%03X Y:0x%03X"),touchX,touchY);
	#endif
	tftFillRectangle(DWIDTH-CBEGIN,CBEGIN,DWIDTH-CEND,CEND,BLACK);
	//-------------------Calculation---------------------
	_x1++; //Add 1 and divide by 2 later = +0.5 (for correct rounding)
	_y1++;
	_x2++;
	_y2++;
	_x1>>=1; //Divide by 2
	_y1>>=1;
	_x2>>=1;
	_y2>>=1;
	dx = (_x2-_x1);	//Build the Difference
	dy = (_y2-_y1);
	#ifdef TOUCH_DEBUG
		tftPrintf_P(2,HHEIGHT-6,WHITE,BLUE, SmallFont, PSTR("X1:0x%03X Y1:0x%03X DX:0x%03X DY:0x%03X"),_x1,_y1,dx,dy);
		calibration=1;
		while(calibration);
	#endif
	tftClear(BLACK); //Finish
	//The Pixel Value is later calculated with a modified version of the basic therory about linear math functions
}
ISR (USARTD1_RXC_vect)	//SPI Tranmission Finish Interrupt
{
	static unsigned char state=0; //State 
	static unsigned int temp;
	#if TOUCH_PRECISION > 255
	     static unsigned int run_number=0;
	#else 
		static unsigned char run_number=0;
	#endif
	#if TOUCH_PRECISION > 16 
		static unsigned long sum=0;
	#else
		static unsigned int sum=0;
	#endif
	static unsigned int resX; //Result X (unconverted)
	static unsigned int resY; //Result Y (unconverted)
	static BOOL penDown;
	static BOOL oldPenDown=0;
	switch(state)
	{
		case 0:	//Same Code for State 0 and 3
		case 3:
			//Called after Starting a Conversation
			USARTD1.DATA; // Read SPI Data (Necessary for AVR Platfrom)
			USARTD1.DATA = 0x00; //Send a zero (and receive the first databyte later)
			state++; //Go to next state (at next Interrupt)
		break;
		case 1: //Same Code for State 1 and 4
		case 4:
			temp = ((unsigned int)USARTD1.DATA<<5);  //Save First Databyte (See Manual for Details about shifting)
			USARTD1.DATA = 0x00; //Send a zero again (and receive the second and last databyte	later)
			state++;
		break;
		case 2:
			temp|=((unsigned int)USARTD1.DATA>>3); //Save Second Databyte
			sum+=temp;	//Add Value to Sum (for Average)
			if(run_number++< (TOUCH_PRECISION-1)) //If NOT Enough Values taken
			{
				USARTD1.DATA=0x90;  //Start another X Conversation
				state=0;	   //Repeat States
			}
			else  //Enough Values received
			{
				USARTD1.DATA=0xD0; // Start first Y Conversation
				resX=sum / TOUCH_PRECISION;	//Calculate X Average
				run_number=0; 
				sum=0;
				state++; //Go on
			}
		break;
		case 5:
			temp|=((unsigned int)USARTD1.DATA>>3);
			sum+=temp;
			if(run_number++< (TOUCH_PRECISION-1))  //If NOT Enough Values taken
			{
				USARTD1.DATA=0xD0;  //Start another Y Conversation
				state=3; //Repeat States
			}
			else //Enough Values received
			{
				resY=sum / TOUCH_PRECISION;	//Calculate Y Average
				run_number=0; //Reset Values for next Time
				sum=0;
				state=0;
				SET_CS_TOUCH; //Disable Touch 
				penDown=(resX!=0xFFF); //Flag if Pen is down
				//------------------Analysis------------------
				if(calibration)	//If in Calibration mode
				{
					
					if(penDown)
					{
						touchX=resX;  //Set Touch X directly (without Conversation)
						touchY=resY;
						TCD1.CTRLA = TC_CLKSEL_DIV256_gc;  //Activate Timer
					}
					else
					{
						
						if(oldPenDown) //Run only if we got at least one pen down
							calibration=0;	 //Calibration finish (Touch X and Y are the values from the last measure, where the pen was down) 				
						PORTD.INTFLAGS = PORT_INT0IF_bm; // Clear Interrupt Flag (For preventing an other interrupt after activation)
						PORTD.INTCTRL = PORT_INT0LVL_LO_gc;
					}
				
				}
				else  //Normal Mode
				{
					PTask_QueueEvent(touchTask,6,penDown,oldPenDown,resX>>8,resX&0x00FF,resY>>8,resY&0x00FF);
					if(penDown)
					{	
						TCD1.CNT = 0x00;
						TCD1.CTRLA = TC_CLKSEL_DIV256_gc;  //Activate Timer
					}					
					else
					{
						PORTD.INTFLAGS = PORT_INT0IF_bm; // Clear Interrupt Flag (For preventing an other interrupt after activation)
						PORTD.INTCTRL = PORT_INT0LVL_LO_gc;
					}
					
							
				}
				oldPenDown=penDown;	//Set old value (for edge detection)
				//tftPrintf_P(10,HHEIGHT-10,WHITE,BLACK,SmallFont,PSTR("penDown (%1u)"),penDown);
			}
		break;
	}
}
 
BOOL touchHaveEmpty(unsigned char num)
{
	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==NULL) num--;
		if(num==0) return TRUE;
	}
	return FALSE;	

}
BOOL touchRegisterArea(TOUCH_AREA_STRUCT* area) //Registers an Area (fill Struct first). Return FALSE if no more Space in the Pointertable (-->Change NUM_AREAS).
{

	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==NULL)
		{
			area->flags=0;
			areas[i]=area;
			return TRUE;
		}		
	}		
	return FALSE;
}

void touchUnregisterArea(TOUCH_AREA_STRUCT* area)//Unregisters an Area
{
	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==area)
		{
			areas[i]=NULL;
			break;
		}		
	}		
}