#include"ll_touch.h"
#include<stm32f4xx_spi.h>
#include<stm32f4xx_rcc.h>

#define CLEAR_CS    GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define SET_CS      GPIO_SetBits(GPIOB,GPIO_Pin_9)

static uint8_t touch_send(uint8_t dat)
{
  SPI_I2S_SendData(SPI2, dat);
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) != RESET);  
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  return  SPI_I2S_ReceiveData(SPI2);                                    
}

bool ll_touch_init() 
{
    //We have a ADS7843 Touch controller
    //Datasheet: http://www.ti.com/lit/ds/symlink/ads7843.pdf

    // init structures
    SPI_InitTypeDef     SPI_SPI2_InitStructure;
    GPIO_InitTypeDef    GPIO_SPI2_InitStructure;

    // enable gpio clock
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    // enable spi clock 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    // fill gpio init struct and init gpio
    GPIO_StructInit(&GPIO_SPI2_InitStructure);
    GPIO_SPI2_InitStructure.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;     // 10 = SPI2_SCK, 14 = SPI2_MISO, 15 = SPI2_MOSI
    GPIO_SPI2_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_SPI2_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_SPI2_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_SPI2_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);
    
    // set chip select
    SET_CS;

    // fill gpio init struct and init gpio
    GPIO_StructInit(&GPIO_SPI2_InitStructure);
    GPIO_SPI2_InitStructure.GPIO_Pin    = GPIO_Pin_9;                                  // 9 = SPI2_CS
    GPIO_SPI2_InitStructure.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_SPI2_InitStructure.GPIO_OType  = GPIO_OType_PP;
    GPIO_SPI2_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);

    // init alternate functions on GPIOB
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

    // clear spi initialisation
    SPI_I2S_DeInit(SPI2);

    // fill spi init structure
    SPI_StructInit(&SPI_SPI2_InitStructure);
    SPI_SPI2_InitStructure.SPI_Direction            = SPI_Direction_2Lines_FullDuplex;
    SPI_SPI2_InitStructure.SPI_Mode                 = SPI_Mode_Master;
    SPI_SPI2_InitStructure.SPI_DataSize             = SPI_DataSize_8b;
    SPI_SPI2_InitStructure.SPI_CPOL                 = SPI_CPOL_Low;
    SPI_SPI2_InitStructure.SPI_CPHA                 = SPI_CPHA_1Edge;
    SPI_SPI2_InitStructure.SPI_NSS                  = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    SPI_SPI2_InitStructure.SPI_BaudRatePrescaler    = SPI_BaudRatePrescaler_256;
    SPI_SPI2_InitStructure.SPI_FirstBit             = SPI_FirstBit_MSB;

    // init spi
    SPI_Init(SPI2, &SPI_SPI2_InitStructure);  
 
    // enable spi   
    SPI_Cmd(SPI2, ENABLE);


    /*
     * TEST ahead ...
     */

    CLEAR_CS;
    touch_send(0x90);
    
    for(long i=0; i<1000; i++);

    uint16_t buf = ((uint16_t) touch_send(0x00))<<5;
    buf|= touch_send(0x00) >> 3;

    SET_CS;


    return true;
}

