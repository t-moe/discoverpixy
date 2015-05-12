#include "ll_touch.h"

void ll_touch_read(const u16 RegisterStartAddress, const u8 NumberOfRegisters, u16 *DataBuffer, const u16 OffsetInBuffer)
{
  u16 ControlValue;
  u8 i,j, RegisterIndex;
  u16 InputValue;
  
  ControlValue = 0xE400 | (RegisterStartAddress & 0x03FF);                      // Create the 16-bit header
  
  for (i=0;i<100;i++);                                                          // delay

  //Write out the control word
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                                            // CS low
  SPI_I2S_SendData(SPI1, ControlValue);
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);  
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  InputValue = SPI_I2S_ReceiveData(SPI1);                                       // Read dummy

  //Copy each register in the buffer contiguously.
  //Read data in.
  for (RegisterIndex=0; RegisterIndex<NumberOfRegisters; RegisterIndex++){
    SPI_I2S_SendData(SPI1, 0xFFFF);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET); 
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    InputValue = SPI_I2S_ReceiveData(SPI1);                                     // read data
    *(DataBuffer+OffsetInBuffer+RegisterIndex)=(u16)InputValue;
  }

  GPIO_SetBits(GPIOA, GPIO_Pin_4);                                              // CS high
  for (j=0;j<5;j++);                                                            // delay
}

void ll_touch_send_cmd(const u16 RegisterAddress, const u8 NumberOfRegisters, u16 *DataBuffer, const u8 OffsetInBuffer)
{
  u16 ControlValue;
  u16 ValueToWrite;
  u16 RegisterIndex;
  u8 j;
  
  ControlValue = 0xE000 | (RegisterAddress & 0x03FF);                           //Create the 16-bit header

  //Write out the control word
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);                                            // CS low
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

  GPIO_SetBits(GPIOA, GPIO_Pin_4);                                              // CS high
  for (j=0;j<5;j++);                                                            // delay
}

bool ll_touch_init() 
{
    // init structures
    SPI_InitTypeDef     SPI_SPI1_InitStructure;
    GPIO_InitTypeDef    GPIOSPI1_InitStructure;

    // enable gpio clock    TODO: Check if not done already
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // enable spi clock     TODO: Check if this is the correct one
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIOSPI1_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;     // 5 = SPI1_SCK, 6 = SPI1_MISO, 7 = SPI1_MOSI
    GPIOSPI1_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOSPI1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOSPI1_InitStructure);

    GPIOSPI1_InitStructure.GPIO_Pin = GPIO_Pin_4;                               // 4 = SPI1_CS
    GPIOSPI1_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOSPI1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOSPI1_InitStructure);

    // clear spi initialisation
    SPI_I2S_DeInit(SPI1);

    // fill spi init structure
    SPI_SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_SPI1_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_SPI1_InitStructure.SPI_CRCPolynomial = 7;  

    // init spi
    SPI_Init(SPI1, &SPI_SPI1_InitStructure);  
    SPI_Cmd(SPI1, ENABLE);

    return true;
}

