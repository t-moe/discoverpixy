/*  TFT lowlevel functions
 *  
 *  Pinout:
 *  ----------------------------------------
 *  PB0  -> LCD_Backlight   PE3  -> LCD_RS
 *  PD0  -> LCD_DB2         PE7  -> LCD_DB4
 *  PD1  -> LCD_DB3         PE8  -> LCD_DB5
 *  PD4  -> LCD_RD          PE9  -> LCD_DB6
 *  PD5  -> LCD_WR          PE10 -> LCD_DB7
 *  PD7  -> LCD_CS          PE11 -> LCD_DB10
 *  PD8  -> LCD_DB15        PE12 -> LCD_DB11
 *  PD9  -> LCD_DB16        PE13 -> LCD_DB12
 *  PD10 -> LCD_DB17        PE14 -> LCD_DB13
 *  PD14 -> LCD_DB0         PE15 -> LCD_DB14
 *  PD15 -> LCD_DB1
 *  ----------------------------------------
 */
#include "ll_tft.h"
#include "tft.h"
#include "system.h"
#include "stm32f4xx.h"
#include "font.h"
#include "stdlib.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_fsmc.h"

/*
 * ---------------------- prototypes --------------------------------------------------------------
 */

// init functions
bool ll_fsmc_init();
bool ll_gpio_init();
bool ll_display_init();

// display control functions
void ll_tft_write_reg(uint8_t reg_adr, uint16_t reg_value);
uint16_t ll_tft_read_reg(uint8_t reg_adr);

/*
 * ---------------------- defines and makros ------------------------------------------------------
 */

// Colors
#define DISPLAY_COLOR_BLACK          0x0000
#define DISPLAY_COLOR_BLUE           0x001F
#define DISPLAY_COLOR_GREEN          0x07E0
#define DISPLAY_COLOR_RED            0xF800
#define DISPLAY_COLOR_WHITE          0xFFFF
#define DISPLAY_COLOR_CYAN           0x07FF
#define DISPLAY_COLOR_MAGENTA        0xF81F
#define DISPLAY_COLOR_YELLOW         0xFFE0
#define DISPLAY_COLOR_GREY           0xF7DE

// FSMC adresses 
#define TFT_REG  (*((volatile unsigned short *) 0x60000000)) // RS = 0
#define TFT_RAM  (*((volatile unsigned short *) 0x60100000)) // RS = 1
#define TFT_RAM_ADR                             0x60100000   // RAM adress

// Display defines
#define TFT_SSD1289_ID1         0x1289              // ID -> SSD1289 (New)
#define TFT_SSD1289_ID2         0x8989              // ID -> SSD1289 (Old)
#define TFT_MAXX                240                 // Number of pixels x axis
#define TFT_MAXY                320                 // Number of pixels y axis
#define TFT_PIXEL               TFT_MAXX*TFT_MAXY   // Total number of pixels
#define TFT_SSD1289_FSMC_AST    15                  // AdressSetupTime  (AST >= 9)
#define TFT_SSD1289_FSMC_DST    15                  // DataSetupTime    (DST >= 7)
#define TFT_SSD1289_PORTRAIT    0x6830              // Mode = Portrait
#define TFT_SSD1289_LANDSCAPE   0x6838              // Mode = Landscape
#define CURRENT_MODE            TFT_SSD1289_LANDSCAPE

// Display controller adresses
#define TFT_SSD1289_REG_00      0x00                // Display-ID register
#define TFT_SSD1289_REG_11      0x11                // Display-Mode register
#define TFT_SSD1289_REG_4E      0x4E                // Cursor-Pos (x) register
#define TFT_SSD1289_REG_4F      0x4F                // Cursor-Pos (y) register
#define TFT_SSD1289_REG_22      0x22                // RAM start register
#define TFT_SSD1289_REG_44      0x44                // X-start+end register
#define TFT_SSD1289_REG_45      0x45                // Y-start register
#define TFT_SSD1289_REG_46      0x46                // Y-end register

// Timeouts
#define TFT_INIT_TIMEOUT        1                   // 1ms timeout 

/*
 * ---------------------- init functions ----------------------------------------------------------
 */

bool ll_tft_init()
{
    bool gpio, fsmc, display;

    // init gpio
    gpio = ll_gpio_init();
    // delay
    system_delay(TFT_INIT_TIMEOUT);
    // init fsmc
    fsmc = ll_fsmc_init();
    // delay
    system_delay(TFT_INIT_TIMEOUT);
    // init display
    display = ll_display_init();
    
    return (gpio & fsmc & display);
}

bool ll_display_init() 
{
    ll_tft_reset(true); // toggle reset
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_reset(false);

    ll_tft_write_reg(0x0007,0x0021);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0000,0x0001);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0007,0x0023);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0010,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0007,0x0033);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(TFT_SSD1289_REG_11,0x6018); // set mode (landscape, portrait)
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0002,0x0600);
    system_delay(TFT_INIT_TIMEOUT);
    //ll_tft_write_reg(0x0012,0x6CEB);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0003,0xA8A4);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x000C,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x000D,0x080C);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x000E,0x2B00);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x001E,0x00B0);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0001,0x2b3F);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0005,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0006,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0016,0xEF1C);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0017,0x0103);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x000B,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x000F,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0041,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0042,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0048,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0049,0x013F);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x004A,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x004B,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0044,0xEF00);  // horizontal start and end
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0045,0x0000);  // vertical start
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0046,0x013F);  // vertical end
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0030,0x0707);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0031,0x0204);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0032,0x0204);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0033,0x0502);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0034,0x0507);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0035,0x0204);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0036,0x0204);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0037,0x0502);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x003A,0x0302);
    system_delay(TFT_INIT_TIMEOUT);
    //ll_tft_write_reg(0x002F,0x12BE);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x003B,0x0302);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0023,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0024,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x0025,0x8000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x004f,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    ll_tft_write_reg(0x004e,0x0000);
    system_delay(TFT_INIT_TIMEOUT);
    TFT_REG = TFT_SSD1289_REG_22;

    return true;
}

bool ll_fsmc_init()
{
    // generate init structures
    FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;
    // clock enable 
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);  
    // prepare timing struct 
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = TFT_SSD1289_FSMC_AST;
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 1;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = TFT_SSD1289_FSMC_DST;
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;
    // Bank-1 / PSRAM-1
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;  
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure; 
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
    // config FSMC
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    // enable Bank-1 / PSRAM-1
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

    return true;
}

bool ll_gpio_init()
{
    // generate init structure
    GPIO_InitTypeDef GPIO_InitStructure;

    // clock enable PORT_B, PORT_D, PORT_E
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    
    // PORT_B init -------------------------------------------------------------------------------------
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;              
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    // configure PORT_B
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    // PORT_D init -------------------------------------------------------------------------------------
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC); // PD0=FSMC_D2   -> DB2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC); // PD1=FSMC_D3   -> DB3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC); // PD4=FSMC_NOE  -> RD
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC); // PD5=FSMC_NWE  -> WR
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,  GPIO_AF_FSMC); // PD7=FSMC_NE1  -> CS
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC); // PD8=FSMC_D13  -> DB15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC); // PD9=FSMC_D14  -> DB16
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC); // PD10=FSMC_D15 -> DB17  
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC); // PD14=FSMC_D0  -> DB0
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC); // PD15=FSMC_D1  -> DB1
    // PORT_D struct
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    // configure PORT_D
    GPIO_Init(GPIOD, &GPIO_InitStructure);   
    
    // PORT_E init --------------------------------------------------------------------------------------
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource3,  GPIO_AF_FSMC); // PE3=FSMC_A19  -> RS
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7,  GPIO_AF_FSMC); // PE7=FSMC_D4   -> DB4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8,  GPIO_AF_FSMC); // PE8=FSMC_D5   -> DB5  
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9,  GPIO_AF_FSMC); // PE9=FSMC_D6   -> DB6
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC); // PE10=FSMC_D7  -> DB7
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC); // PE11=FSMC_D8  -> DB10
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC); // PE12=FSMC_D9  -> DB11
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC); // PE13=FSMC_D10 -> DB12
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC); // PE14=FSMC_D11 -> DB13
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC); // PE15=FSMC_D12 -> DB14
    // PORT_E struct
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    // configure PORT_E
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    return true;
}

/*
 * ---------------------- GPIO control functions ----------------------------------------------------------
 */

// TODO

/*
 * ---------------------- display control functions -------------------------------------------------------
 */

void ll_tft_set_cursor(uint16_t xpos, uint16_t ypos)
{
    // set cursor
    ll_tft_write_reg(TFT_SSD1289_REG_4E, ypos);
    ll_tft_write_reg(TFT_SSD1289_REG_4F, 319-xpos);
    TFT_REG = TFT_SSD1289_REG_22;
}

void ll_tft_set_backlight(bool state) 
{
    if(state){
        GPIOB->BSRRH = GPIO_Pin_0;
    } else {
        GPIOB->BSRRL = GPIO_Pin_0;    
    }
}

void ll_tft_reset(bool state) 
{
    if(state){
        GPIOB->BSRRH = GPIO_Pin_0;
    } else {
        GPIOB->BSRRL = GPIO_Pin_0;    
    }
}

void ll_tft_clear(uint16_t color) 
{
    uint32_t n = 0;
    
    // set cursor to 0
    ll_tft_set_cursor(0,0);
    
    for(n = 0; n < TFT_PIXEL; n++) {
        TFT_RAM = color;
    }
}

void ll_tft_write_reg(uint8_t reg_adr, uint16_t reg_value)
{
    TFT_REG = reg_adr;
    TFT_RAM = reg_value;  
}

uint16_t ll_tft_read_reg(uint8_t reg_adr)
{
    TFT_REG = reg_adr;
    return TFT_RAM;
}

void ll_tft_set_window(uint16_t xstart, uint16_t ystart, uint16_t xend, uint16_t yend)
{
    uint16_t start,end;
    uint16_t ystart_end;

    start = (ystart & 0x00FF);
    end = ((yend & 0x00FF) << 8);
    ystart_end = (start | end);

    ll_tft_write_reg(TFT_SSD1289_REG_44, ystart_end);  
    ll_tft_write_reg(TFT_SSD1289_REG_45, 319-xend);
    ll_tft_write_reg(TFT_SSD1289_REG_46, 319-xstart);
}

void ll_tft_reset_window()
{
    ll_tft_write_reg(0x44,239<<8);
    ll_tft_write_reg(0x45,0);
    ll_tft_write_reg(0x46,319);
}

/*
 * ---------------------- draw functions -----------------------------------------------------------
 */

void ll_tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
    
    if(y1==y2){
        if(x2<x1){
            ll_tft_set_cursor(x2,y1);
            do{
                TFT_RAM = color;        //Let Semi away, because of block makro
            } while(x2++!=x1);
        } else {
            ll_tft_set_cursor(x1,y1);
            do{
                TFT_RAM = color;        //Let Semi away, because of block makro
            } while(x1++!=x2);
        }       
    }
    else if(x1==x2){
           ll_tft_write_reg(0x11,0x6030); // Change adresspointer direction
          
          // (is quicker than defining a region)
          if(y2<y1)
          {
               ll_tft_set_cursor(x1,y2);
               do {
                    TFT_RAM = color;        //Let Semi away, because of block makro
               } while(y2++!=y1);
          }
          else
          {
               ll_tft_set_cursor(x1,y1);
               do {
                    TFT_RAM = color;        //Let Semi away, because of block makro
               }while(y1++!=y2);
          }
           ll_tft_write_reg(0x11,0x6018); // Set adresspointer direction normal again
    }
    else
    {
        if(abs(x2-x1) > abs(y2-y1))
        {   
            //Without floating point!
            int deltax = ((int)x2-(int)x1);
            int deltay = ((int)y2-(int)y1)<<1;  // multiple by 2 to make it easier to round
            int x = 0;
            if (x1>x2)
            {
                do
                { 
                    //y = mx + b (math theory, linear functions)
                    ll_tft_set_cursor(x1+x,y1+ (((long)deltay*(long)x/deltax+1)>>1)); // Add 1 and divde by 2 = +0.5
                    TFT_RAM = color;
                }
                while(x--!=deltax);
            }
            else
            {
                do
                { 
                    ll_tft_set_cursor(x1+x,y1+ (((long)deltay*(long)x/deltax+1)>>1));
                    TFT_RAM = color;
                }
                while(x++!=deltax);
            }
        }
        else
        {
            int deltax = ((int)x2-(int)x1)<<1;
            int deltay = ((int)y2-(int)y1);
            int y = 0;
            if (y1>y2)
            {
                do
                { 
                    ll_tft_set_cursor(x1+ (((long)deltax*(long)y/deltay+1)>>1),y1+ y);
                    TFT_RAM = color;
                }
                while(y--!=deltay);
            }
            else
            {
                do
                { 
                    ll_tft_set_cursor(x1+ (((long)deltax*(long)y/deltay+1)>>1),y1+ y);
                    TFT_RAM = color;
                }
                while(y++!=deltay);
            }   
        } 
    }    
}

void ll_tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color) 
{
    ll_tft_set_cursor(x,y);
    TFT_RAM = color;
}

void ll_tft_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) 
{
    unsigned int tmp;
    unsigned int i;

    if(x1 > x2){
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if(y1 > y2){
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    i = x1;

    ll_tft_set_cursor(x1, y1);
    
    while(i++ != x2) TFT_RAM = color;
    
    ll_tft_set_cursor(x1,y2);
    
    while(i-- != x1) TFT_RAM = color;
    
    i = y1;
    
    ll_tft_write_reg(0x11,0x6030); // Change adresspointer direction
    ll_tft_set_cursor(x2, y1);
    
    while(i++ != y2) TFT_RAM = color;
    
    ll_tft_set_cursor(x1, y1);
    
    while(i-- != y1) TFT_RAM = color;
    ll_tft_write_reg(0x11,0x6018); // Set adresspointer direction normal again
}

void ll_tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color)
{
    uint16_t area;
    uint32_t n;
    
    // set window 
    ll_tft_set_window(x1, y1, x2, y2);
    ll_tft_set_cursor(x1, y1);

    // calculate area
    area = (x2 - x1 + 1) * (y2 - y1 + 1);        

    // fill area
    for(n = 0; n < area; n++) {
        TFT_RAM = color;
    }
    
    ll_tft_reset_window();
}

void ll_tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat)
{
    // TODO
}

void ll_tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    // TODO
}

const char *get_font(uint8_t font)
{
    switch(font){
        case 0: return small_font;
        case 1: return big_font; 
        case 2: return seven_seg_num_font;
    }
}

uint8_t ll_tft_num_fonts() 
{
	return 3;
}

uint8_t ll_tft_font_height(uint8_t fontnum) {
    const char *font = get_font(fontnum);
    return (uint8_t) font[1];
}

uint8_t ll_tft_font_width(uint8_t fontnum) {
    const char *font = get_font(fontnum);
    return (uint8_t) font[0];
}

void ll_tft_draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t fontnum, char c) 
{

    const char *font        = get_font(fontnum);
    unsigned char width     = (uint8_t) font[0];
    unsigned char height    = (uint8_t) font[1];
    unsigned char offset    = (uint8_t) font[2];
    unsigned int ind        = ((c-offset) * ((width / 8) * height)) + 4;
    unsigned int cnt        = 0;
    unsigned char bitm      = 0;

    bool bgIsTrans          = (bgcolor == TRANSPARENT);
    bool enTrans            = 0;
    
    ll_tft_set_window(x, y, x + width - 1, y + height - 1);
    ll_tft_set_cursor(x, y);   
 
    for(cnt = (width / 8) * height; cnt > 0; cnt--){
        for(bitm = 0x80; bitm > 0; bitm >>= 1){
            if((font[ind]) & bitm){
                if(enTrans){
                    enTrans = 0;
                    ll_tft_write_reg(0x23,0x0000); 
                    ll_tft_write_reg(0x24,0x0000); 
                    TFT_REG = TFT_SSD1289_REG_22;
                }

                TFT_RAM = color;
            
            } else {
                if(bgIsTrans && !enTrans){
                    enTrans = 1;
                    ll_tft_write_reg(0x23,0xFFFF); 
                    ll_tft_write_reg(0x24,0xFFFF); 
                    TFT_REG = TFT_SSD1289_REG_22;
                }

               TFT_RAM = bgcolor; 
            }
        }
        
        ind++;
    }
    
    if(enTrans){
        ll_tft_write_reg(0x23,0x0000);
        ll_tft_write_reg(0x24,0x0000); 
    }

    ll_tft_reset_window();
}
