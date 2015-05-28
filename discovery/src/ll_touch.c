#include"ll_touch.h"
#include<stm32f4xx_spi.h>
#include<stm32f4xx_rcc.h>
#include<stm32f4xx_exti.h>
#include<stm32f4xx_syscfg.h>
#include<tft.h>
#include<touch.h>
#include<stdio.h>
#include<stdlib.h>

/* Defines ----------------------------------------------------------  */
#define CLEAR_CS        GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define SET_CS          GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define REQ_X_COORD     0x90    // Request x coordinate
#define REQ_Y_COORD     0xD0    // Request y coordinate
#define REQ_1_DATAB     0x00    // Request one databyte
#define DWIDTH          320
#define DHEIGHT         240
#define CCENTER         20
#define x1              0x0231
#define dx              0x0C08
#define y1              0x0287
#define dy              0x0B56
#define NSAMPLE         16

/* Globals ----------------------------------------------------------- */
volatile bool pen_state     = false;    // PenDown = True; PenUp = False;
volatile bool tim_flag      = false;
volatile uint16_t x_samples[NSAMPLE-1];
volatile uint16_t y_samples[NSAMPLE-1];
volatile int i;

/* Prototypes -------------------------------------------------------- */
bool ll_touch_init();
static void     init_exti();
static void     init_timer();
static uint8_t  touch_send(uint8_t dat);
static uint16_t avg_vals(uint16_t samples[], uint16_t len);
static uint16_t touch_get_y_coord();
static uint16_t touch_get_y_coord();
void touch_test(uint16_t x, uint16_t y);

/* Functions --------------------------------------------------------- */
static uint16_t avg_vals(uint16_t samples[], uint16_t len)
{
    uint16_t j = 0;
    uint32_t tmp = 0;

    for(j = 0; j < len; j++){
        tmp += samples[j];    
    }

    return (uint16_t)(tmp/len);
}

static uint16_t touch_get_x_coord()
{
    uint16_t buf_x = 0;
    CLEAR_CS;                                           // clear chipselect
    touch_send(REQ_X_COORD);                            // request x coordinate
    
    buf_x = ((uint16_t) touch_send(REQ_1_DATAB)) << 5;    
    buf_x |= touch_send(REQ_1_DATAB) >> 3;

    SET_CS;                                             // set chipselect

    return buf_x;
}

static uint16_t touch_get_y_coord()
{
    uint16_t buf_y = 0;

    CLEAR_CS;                                           // clear chipselect
    touch_send(REQ_Y_COORD);                            // request y coordinate
    
    buf_y  = ((uint16_t) touch_send(REQ_1_DATAB)) << 5;
    buf_y |= touch_send(REQ_1_DATAB) >> 3;

    SET_CS;                                             // set chipselect

    return buf_y;
}

static uint8_t touch_send(uint8_t dat)
{
  SPI_I2S_SendData(SPI2, dat);
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET);  
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return  SPI_I2S_ReceiveData(SPI2);                                    
}

void touch_test(uint16_t x, uint16_t y)
{
    char xs[10];
    char ys[10];

    tft_clear(BLACK);

    itoa(x, xs, 10);
    itoa(y, ys, 10);

    tft_print_line(10, 10, WHITE, TRANSPARENT, 0, (const char*)xs);
    tft_print_line(10, 30, WHITE, TRANSPARENT, 0, (const char*)ys);
}

bool ll_touch_init() 
{
    //We have a ADS7843 Touch controller
    //Datasheet: http://www.ti.com/lit/ds/symlink/ads7843.pdf

    /* init structures */
    SPI_InitTypeDef     SPI_SPI2_InitStructure;
    GPIO_InitTypeDef    GPIO_SPI2_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   // enable clock on gpiob
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);    // ebable spi clock

    /* fill gpio init struct and init gpio */
    GPIO_StructInit(&GPIO_SPI2_InitStructure);
    GPIO_SPI2_InitStructure.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;     // 10 = SPI2_SCK, 14 = SPI2_MISO, 15 = SPI2_MOSI
    GPIO_SPI2_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_SPI2_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_SPI2_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_SPI2_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);
    
    SET_CS; // set chip select

    /* fill gpio init struct and init gpio */
    GPIO_StructInit(&GPIO_SPI2_InitStructure);
    GPIO_SPI2_InitStructure.GPIO_Pin    = GPIO_Pin_9;                                  // 9 = SPI2_CS
    GPIO_SPI2_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_SPI2_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_SPI2_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);

    /* init alternate functions on GPIOB */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
    
    SPI_I2S_DeInit(SPI2); // Clear spi initialisation

    /* fill spi init structure */
    SPI_StructInit(&SPI_SPI2_InitStructure);
    SPI_SPI2_InitStructure.SPI_Direction            = SPI_Direction_2Lines_FullDuplex;
    SPI_SPI2_InitStructure.SPI_Mode                 = SPI_Mode_Master;
    SPI_SPI2_InitStructure.SPI_DataSize             = SPI_DataSize_8b;
    SPI_SPI2_InitStructure.SPI_CPOL                 = SPI_CPOL_Low;
    SPI_SPI2_InitStructure.SPI_CPHA                 = SPI_CPHA_1Edge;
    SPI_SPI2_InitStructure.SPI_NSS                  = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    SPI_SPI2_InitStructure.SPI_BaudRatePrescaler    = SPI_BaudRatePrescaler_256;
    SPI_SPI2_InitStructure.SPI_FirstBit             = SPI_FirstBit_MSB;

    SPI_Init(SPI2, &SPI_SPI2_InitStructure);    // init spi
    SPI_Cmd(SPI2, ENABLE);                      // enable spi

    init_exti();                                // init external interrupt for penirq
    init_timer();                               // init the timer 6 for sampling x and y coordinates

    return true;
}

static void init_exti()
{
    /* init structures */
    GPIO_InitTypeDef gpio;
    EXTI_InitTypeDef exti;
    NVIC_InitTypeDef nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   // enable GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  // enable SYSCFG clock

    /* Set GPIOA0 as input */
    gpio.GPIO_Mode   = GPIO_Mode_IN;
    gpio.GPIO_OType  = GPIO_OType_PP;
    gpio.GPIO_Pin    = GPIO_Pin_0;
    gpio.GPIO_PuPd   = GPIO_PuPd_UP;
    gpio.GPIO_Speed  = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gpio);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); // Bind Exti_line0 to PA0

    /* EXTI on PA0 */
    EXTI_StructInit(&exti);
    exti.EXTI_Line      = EXTI_Line0;
    exti.EXTI_Mode      = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger   = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd   = ENABLE;
    EXTI_Init(&exti);

    /* Add IRQ vector to NVIC */
    nvic.NVIC_IRQChannel = EXTI0_IRQn;               // PD0 -> EXTI_Line0 -> EXTI0_IRQn vector
    nvic.NVIC_IRQChannelPreemptionPriority = 0x00;   // Set priority
    nvic.NVIC_IRQChannelSubPriority = 0x00;          // Set sub priority
    nvic.NVIC_IRQChannelCmd = ENABLE;                // Enable interrupt
    NVIC_Init(&nvic);                                // Config NVIC
}

static void init_timer()
{
    TIM_TimeBaseInitTypeDef t;
    const int APB1_CLK = 42E6;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    // Enable clock for TIM6

    /* Timer 7 configuration */
    TIM_TimeBaseStructInit(&t);                             // Init TimeBaseStruct
    t.TIM_Prescaler = APB1_CLK / 1000 - 1;                  // 0..41999 prescaler
    t.TIM_Period = 20- 1;                                   // 10ms cycle time
    TIM_TimeBaseInit(TIM7, &t);                             // Init TIM7
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);              // Enable update IRQ for TIM7
    NVIC_EnableIRQ(TIM7_IRQn);                              // Enable IRQs for TIM7
}

/* Interrupt service routines ------------------------------------------ */
void EXTI0_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET) {    // Make sure the interrupt flag is set

        if(!pen_state){                             // Check if PENDOWN or PENUP
            TIM_Cmd(TIM7, ENABLE);                  // Start the timer
            while(!tim_flag);                       // Wait for the sampling to finish
            touch_add_raw_event(avg_vals(x_samples, NSAMPLE), avg_vals(y_samples, NSAMPLE), TOUCH_DOWN);
        }else{
            TIM_Cmd(TIM7, ENABLE);                  // Start the timer
            while(!tim_flag);                       // Wait for the sampling to finish
            touch_add_raw_event(avg_vals(x_samples, NSAMPLE), avg_vals(y_samples, NSAMPLE), TOUCH_UP);
        }
 
        pen_state = !pen_state;                     // Toggle penstate
        tim_flag = false;                           // Clear timer flag
        EXTI_ClearITPendingBit(EXTI_Line0);         // Clear interrupt flag
    }
}

void TIM7_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM7, TIM_IT_Update) == SET){  // Make sure the interrupt flag is set
        for(i = 0; i < (NSAMPLE-1); i++){
            /* get x and y coordinate and apply calibration */
            x_samples[i] = (((long)(DWIDTH-2*CCENTER)*2*(long)((long)touch_get_x_coord()-x1)/dx+1)>>1)+CCENTER;  
            y_samples[i] = (((long)(DHEIGHT-2*CCENTER)*2*(long)((long)touch_get_y_coord()-y1)/dy+1)>>1)+CCENTER;  
        }
        
        tim_flag = true;                                // Set the global timer flag   
        TIM_Cmd(TIM7, DISABLE);                         // Count only once
    }
    
}
