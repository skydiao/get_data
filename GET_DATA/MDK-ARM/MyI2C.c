#include "stm32f4xx.h"                 
#include "MyI2C.h"
#include "getdata.h"

//初始化IIC
//void IIC_Init(void)
//{	
//	IIC_SCL=1;
//	IIC_SDA=1;
//}

//产生起始信号
void IIC_Start(void)
{
	SDA_OUT();  //SDA线输出
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(4);
	IIC_SDA=0;//时钟线为高电平时，数据总线由高电平变为低电平表示起始信号
	delay_us(4);
	IIC_SCL=0;  //准备发送和接收数据，因为发送数据是在SCL电平期间执行的
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT(); //SDA线输出
	IIC_SCL=0;
	IIC_SDA=0; //SCL为低电平时，SDA从低电平转换为高电平表示停止信号
	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;
	delay_us(4);
}


//等待应答信号的到来
//返回值：1，接收应答失败
//		  0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();  //SDA设置为输入
	IIC_SDA=1;
	delay_us(1);
	IIC_SCL=1;
	delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)//防止应答时间过长，程序崩溃
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;
	return 0;
}

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;  //SCL由高电平转到低电平表示一次应答
}
//不产生ACK应答
void IIC_NACK(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;  //SDA高电平期间是不产生应答的
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0; 
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 Byte)
{
	u8 i;
	SDA_OUT();
	IIC_SCL=0;  //拉低时钟开始数据传输
	for(i=0;i<8;i++)
	{
		IIC_SDA=(Byte&0x80)>>7;
		Byte<<=1;
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);  //时钟由高电平到低电平表示发送一个字节
	}
}
//读字节
//Ack=1，发送ACK
//Ack=0，发送nACK
u8 IIC_Read_Byte(u8 Ack)
{
	u8 i,Data;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;  //读一个字节之前首先释放总线	
		Data<<=1;
		if(READ_SDA)
			Data++;
		delay_us(1);
	}
	if(!Ack)
		IIC_NACK();
	else
		IIC_Ack();
	return Data;
}
 





