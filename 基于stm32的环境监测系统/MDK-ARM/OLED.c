#include "stm32f4xx.h"         
#include "OLED.h"
#include "OLED_Font.h" //这是字库，可以网上找
#include "delay.h"
#include "MyI2C.h"
 
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		IIC_SCL=1;
        delay_us(4);  
		IIC_SCL=0;
		IIC_Byte<<=1;
	}
    IIC_SDA = 1; 
	IIC_SCL=1;
    delay_us(4);
	IIC_SCL=0;
}
 
 
void OLED_WriteData(unsigned char Data)
{
	IIC_Start();
    IIC_Send_Byte(0x78);				
    IIC_Wait_Ack();	
    IIC_Send_Byte(0x40);			//写数据
	IIC_Wait_Ack();	
    IIC_Send_Byte(Data);
	IIC_Wait_Ack();
	IIC_Stop();
}
 
void OLED_WriteCommand(unsigned char Command)
{
	IIC_Start();
    IIC_Send_Byte(0x78);            
	if(IIC_Wait_Ack())
	{
		while(1);
	}
    IIC_Send_Byte(0x00);			
	IIC_Wait_Ack();	
    IIC_Send_Byte(Command); 
	IIC_Wait_Ack();
	IIC_Stop();
}
 
void OLED_Set_Pos(unsigned char x, unsigned char y)  //设置光标位置
{ 
	OLED_WriteCommand(0xb0+y);
	OLED_WriteCommand(((x&0xf0)>>4)|0x10);
	OLED_WriteCommand((x&0x0f)|0x01);
} 
 
void OLED_Fill(unsigned char Data) //清屏
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WriteCommand(0xb0+y);
		OLED_WriteCommand(0x01);
		OLED_WriteCommand(0x10);
		for(x=0;x<128;x++)
		OLED_WriteData(Data);
	}
}
 
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WriteCommand(0xb0+y);
		OLED_WriteCommand(0x01);
		OLED_WriteCommand(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WriteData(0);
	}
}
 
void OLED_Init(void)
{
	HAL_Delay(500); //初始化延迟500ms
	OLED_WriteCommand(0xae);//--turn off oled panel
	OLED_WriteCommand(0x00);//---set low column address
	OLED_WriteCommand(0x10);//---set high column address
	OLED_WriteCommand(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCommand(0x81);//--set contrast control register
	OLED_WriteCommand(0xCF); // Set SEG Output Current Brightness
	OLED_WriteCommand(0xa1);//--Set SEG/Column Mapping     
	OLED_WriteCommand(0xc8);//Set COM/Row Scan Direction   
	OLED_WriteCommand(0xa6);//--set normal display
	OLED_WriteCommand(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WriteCommand(0x3f);//--1/64 duty
	OLED_WriteCommand(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCommand(0x00);//-not offset
	OLED_WriteCommand(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WriteCommand(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCommand(0xd9);//--set pre-charge period
	OLED_WriteCommand(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock   
	OLED_WriteCommand(0xda);//--set com pins hardware configuration
	OLED_WriteCommand(0x12);
	OLED_WriteCommand(0xdb);//--set vcomh
	OLED_WriteCommand(0x40);//Set VCOM Deselect Level
	OLED_WriteCommand(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCommand(0x02);//
	OLED_WriteCommand(0x8d);//--set Charge Pump enable/disable
	OLED_WriteCommand(0x14);//--set(0x10) disable
	OLED_WriteCommand(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteCommand(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WriteCommand(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //清屏
	OLED_Set_Pos(0,0); //设置光标位置
} 
 
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WriteData(F6x8[c][i]);
		x+=6;
		j++;
	}
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void OLED_P6x8Num(unsigned char x,unsigned char y,unsigned int num,int n)
{
	unsigned char c=0,i=0,j=0;
	while(n)
	{
		c = ((uint32_t)(num/OLED_Pow(10,n - 1))%10) + '0' - 32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WriteData(F6x8[c][i]);
		x+=6;
		j++;
		n--;
	}
}

void OLED_P8x16Str(unsigned char x,unsigned  char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WriteData(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WriteData(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
 
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WriteData(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WriteData(F16x16[adder]);
		adder += 1;
	} 	  	
}
 
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;
 
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WriteData(BMP[j++]);
	    }
	}
}

