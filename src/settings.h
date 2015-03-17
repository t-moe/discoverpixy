


// Select Display Controller:
//CONTROLLER_SSD1289	      //Normal 3.2"	240*320
//CONTROLLER_ILI9327	  //Widescreen 3.2" 240*400



#define FLASH_INT_ARR(name, size) prog_uint16_t name[size]	PROGMEM
#define FLASH_INT_ARR_POINTER prog_uint16_t*
#define FLASH_INT_ARR_READ pgm_read_word
	
#define FLASH_CHAR_ARR(name, size) prog_uint8_t name[size]	PROGMEM
#define FLASH_CHAR_ARR_POINTER prog_uint8_t*
#define FLASH_CHAR_ARR_READ pgm_read_byte



#define F_CPU	58000000UL	//für die Delay Funktionen, uC Takt ist 2MHz wenn CLK.CTRL nicht gesetzt wird im Init_Device
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
	
#ifndef NULL
#define NULL ((void *) 0)
#endif

//Sysclock: 8MHz		blablablablablablablablablablablablablablabla
//Data Low: PORTB
//Data High: PORTD
	//Noch nicht umgesetzt:
	//SPI: P0.0 - P0.2  (NSS muss leider freibleiben, kann nicht deaktiviert werden - scheiss Mesa)
	//SPI CS: z.b P4.1 
//PENIRQ (INT0): P0.4
//RS: P0.5
//WR: P0.6
//RESET: P0.7
//CS: P4.0 (Leider nicht mehr bitadressierbar)
	
#define DATAPORT_LOW_DDR	PORTF
#define DATAPORT_HIGH_DDR	PORTB

#define DATAPORT_LOW(val) PORTF.OUT=val
#define DATAPORT_HIGH(val) PORTB.OUT=val
/*
#define RS		5
#define WR		4
//#define RD		PC2
#define CS		7
#define RESET	0
*/
//Akku ON/OFF
#define Batt_ON_bm			0x20		//PortC
//Buzzer
#define Buzzer_bm			0x01		//PortE
//#define Freq_Buzzer_bm		0x01		//PortE
//LED
#define LED_bm				0x10		//PortC
//#define PWM_LED_bm			0x10		//PortC
	
//Battlader an PortA
#define Batt_STAT1_bp		4			//PortA
#define Batt_STAT1_bm		0x10		//PortA
#define Batt_STAT2_bp		6			//PortA
#define Batt_STAT2_bm		0x40		//PortA
#define Batt_STATPG_bp		5			//PortA
#define Batt_STATPG_bm		0x20		//PortA
#define Batt_STATPINS_bm	0x70		//PortA
#define Batt_FastCharge_bp	7			//PortA
#define Batt_FastCharge_bm	0x80		//PortA
#define Batt_STATUS_SHUTDOWN	(Batt_STAT1_bm|Batt_STAT2_bm|Batt_STATPG_bm)	//111
#define Batt_STATUS_STANDBY		(Batt_STAT1_bm|Batt_STAT2_bm)					//110
#define Batt_STATUS_CHARGE		(Batt_STAT2_bm)									//010
#define Batt_STATUS_CHARGEC		(Batt_STAT1_bm)									//100
#define Batt_STATUS_TEMPFAULT	(Batt_STAT1_bm|Batt_STAT2_bm)					//110
#define Batt_STATUS_TESTMODE	0												//000

//Messpunkt für die Akkuspannung der Schaltung @ PortA
#define MP_Batt_bm			0x02
#define MP_Batt_bp			1
//Messpunkt für die Eingangsspannung der Schaltung @ PortA
#define MP_Vin_bm			0x01
#define MP_Vin_bp			0
	
//G Sensor
#define Sensor_Connected_bm		0x08	//PortA
#define Sensor_Connected_bp		3		//PortA
#define Sensor_Interrupt_bm		0x02	//PortR
#define Sensor_Interrupt_bp		1		//PortR
#define Sensor_Reset_bm			0x01	//PortR
#define Sensor_Reset_bp			0		//PortR
#define	Sensor_SPI_CS_bm		0x04	//PortA
#define	Sensor_SPI_DIN_bm		0x04	//PortC		//DIN und DOUT sind im Schema anders!
#define	Sensor_SPI_DOUT_bm		0x08	//PortC		//DIN und DOUT sind im Schema anders!
#define	Sensor_SPI_SCK_bm		0x02	//PortC
#define Sensor_SET_CS			PORTA.OUTSET = Sensor_SPI_CS_bm
#define Sensor_CLR_CS			PORTA.OUTCLR = Sensor_SPI_CS_bm
	
//Display
#define Display_Dataport_L		PORTF	//Die ersten 8 Bit des Datenport
#define Display_Dataport_H		PORTB	//Die letzen 8 Bit des Datenport
#define Display_Backlight_bm	0x01	//PortC
#define Display_CTRL_RS_bm		0x20	//PortE
#define Display_CTRL_WR_bm		0x10	//PortE
#define Display_CTRL_RD_bm		0x08	//PortE
#define Display_CTRL_CS_bm		0x80	//PortC
#define Display_CTRL_RESET_bm	0x01	//PortD
	
//SD
#define SD_SPI_CS_bm		0x40	//PortC
#define SD_SPI_SCK_bm		0x02	//PortD
#define SD_SPI_DOUT_bm		0x08	//PortD			//DIN und DOUT sind im Schema anders!
#define SD_SPI_DIN_bm		0x04	//PortD			//DIN und DOUT sind im Schema anders!
	
//Touch
#define Touch_SPI_CS_bm		0x04	//PortE
#define Touch_SPI_SCK_bm	0x20	//PortD
#define Touch_SPI_DOUT_bm	0x80	//PortD			//DIN und DOUT sind im Schema anders!
#define Touch_SPI_DIN_bm	0x40	//PortD			//DIN und DOUT sind im Schema anders!
#define Touch_Interrupt_bm	0x10	//PortD
#define Touch_Busy_bm		0x02	//PortE
	
	
#define SET_RS 		PORTE.OUTSET = Display_CTRL_RS_bm
#define CLEAR_RS 	PORTE.OUTCLR = Display_CTRL_RS_bm

#define SET_WR 		PORTE.OUTSET = Display_CTRL_WR_bm
#define CLEAR_WR 	PORTE.OUTCLR = Display_CTRL_WR_bm

#define SET_RD 		PORTE.OUTSET = Display_CTRL_RD_bm
#define CLEAR_RD	PORTE.OUTCLR = Display_CTRL_RD_bm

#define SET_CS 		PORTC.OUTSET = Display_CTRL_CS_bm
#define CLEAR_CS 	PORTC.OUTCLR = Display_CTRL_CS_bm
	
#define SET_RESET 	PORTD.OUTSET = Display_CTRL_RESET_bm
#define CLEAR_RESET PORTD.OUTCLR = Display_CTRL_RESET_bm

#define SET_CS_TOUCH	PORTE.OUTSET = Touch_SPI_CS_bm
#define CLEAR_CS_TOUCH	PORTE.OUTCLR = Touch_SPI_CS_bm