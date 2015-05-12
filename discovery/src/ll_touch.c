#include"ll_touch.h"
#include<stm32f4xx_spi.h>
#include<stm32f4xx_rcc.h>


#define CLEAR_CS    GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define SET_CS      GPIO_SetBits(GPIOB,GPIO_Pin_9)


/*
static void touch_read(const uint16_t RegisterStartAddress, const uint8_t NumberOfRegisters, uint16_t *DataBuffer, const uint16_t OffsetInBuffer)
{
  uint16_t ControlValue;
  uint8_t i,j, RegisterIndex;
  uint16_t InputValue;
  
  ControlValue = 0xE400 | (RegisterStartAddress & 0x03FF);                          // Create the 16-bit header
  
  for (i=0;i<100;i++);                                                              // delay

  //Write out the control word
  CLEAR_CS;
  SPI_I2S_SendData(SPI1, ControlValue);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);  
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  InputValue = SPI_I2S_ReceiveData(SPI1);                                           // Read dummy

  //Copy each register in the buffer contiguously.
  //Read data in.
  for (RegisterIndex=0; RegisterIndex<NumberOfRegisters; RegisterIndex++){
    SPI_I2S_SendData(SPI1, 0xFFFF);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET); 
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    InputValue = SPI_I2S_ReceiveData(SPI1);                                         // read data
    *(DataBuffer+OffsetInBuffer+RegisterIndex)=(uint16_t)InputValue;
  }

  SET_CS;
  for (j=0;j<5;j++);                                                                // delay
}

static void touch_send_cmd(const uint16_t RegisterAddress, const uint8_t NumberOfRegisters, uint16_t *DataBuffer, const uint8_t OffsetInBuffer)
{
  uint16_t ControlValue;
  uint16_t ValueToWrite;
  uint16_t RegisterIndex;
  uint8_t j;
  
  ControlValue = 0xE000 | (RegisterAddress & 0x03FF);                               //Create the 16-bit header

  //Write out the control word
  CLEAR_CS;
  SPI_I2S_SendData(SPI1, ControlValue);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);  
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  //And then the data
  for (RegisterIndex=0; RegisterIndex<NumberOfRegisters; RegisterIndex++){
    ValueToWrite= *(DataBuffer+RegisterIndex+OffsetInBuffer);
    SPI_I2S_SendData(SPI1, ValueToWrite);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET); 
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  }

  SET_CS;
  for (j=0;j<5;j++);                                                                // delay
}


*/

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

    return false; //TODO: remove

    // init structures
    SPI_InitTypeDef     SPI_SPI2_InitStructure;
    GPIO_InitTypeDef    GPIO_SPI2_InitStructure;

    // enable gpio clock    TODO: Check if not done already
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // enable spi clock     TODO: Check if this is the correct one
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_SPI2_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;     // 10 = SPI2_SCK, 14 = SPI2_MISO, 15 = SPI2_MOSI
    GPIO_SPI2_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_SPI2_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_SPI2_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);

    GPIO_SPI2_InitStructure.GPIO_Pin = GPIO_Pin_9;                                  // 9 = SPI2_CS
    GPIO_SPI2_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_SPI2_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_SPI2_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_SPI2_InitStructure);

    // clear spi initialisation
    SPI_I2S_DeInit(SPI2);

    // fill spi init structure
    SPI_SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    //SPI_SPI2_InitStructure.SPI_CRCPolynomial = 7;  

    // init spi
    SPI_Init(SPI2, &SPI_SPI2_InitStructure);  
    SPI_Cmd(SPI2, ENABLE);




    //test
    CLEAR_CS;
    touch_send(0x90);
    for(long i=0; i<1000; i++);

    uint16_t buf = ((uint16_t) touch_send(0x00))<<5;
    buf|= touch_send(0x00) >> 3;

    SET_CS;



    return true;
}

