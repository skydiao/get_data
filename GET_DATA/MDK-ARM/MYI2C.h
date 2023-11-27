#ifndef _MYI2C__H_
#define _MYI2C__H_

#include "sys.h"
#define SDA_IN() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}  //PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;}  //PB9输出模式
 
#define IIC_SCL PBout(8) //SCL
#define IIC_SDA PBout(9) //SDA
#define READ_SDA PBin(9) //输出SDA

#define u8 uint8_t
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NACK(void);
void IIC_Send_Byte(u8 Byte);
u8 IIC_Read_Byte(u8 Ack);
 
#endif







