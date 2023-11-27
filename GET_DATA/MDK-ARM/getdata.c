#include "tim.h"
#include "gpio.h"
#include "getdata.h"
#include "stdio.h"
#include "oled.h"
uint16_t speed_left = 500;
uint16_t speed_right = 500;

void go_back()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,speed_right);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,speed_left);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL0_PIN,GPIO_PIN_SET);  
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL1_PIN,GPIO_PIN_RESET);
//	
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL2_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL3_PIN,GPIO_PIN_SET);

}

void go_front()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,speed_right);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,speed_left);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL1_PIN,GPIO_PIN_SET);  
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL0_PIN,GPIO_PIN_RESET);
//	
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL3_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL2_PIN,GPIO_PIN_SET);
}

void go_right()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,speed_left);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL1_PIN,GPIO_PIN_RESET);  
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL0_PIN,GPIO_PIN_RESET);
//	
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL3_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL2_PIN,GPIO_PIN_SET);
}

void go_left()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,speed_right);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL1_PIN,GPIO_PIN_SET);  
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL0_PIN,GPIO_PIN_RESET);
//	
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL3_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL2_PIN,GPIO_PIN_RESET);
}

void car_stop()
{
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL1_PIN,GPIO_PIN_RESET);  
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL0_PIN,GPIO_PIN_RESET);
//	
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL3_PIN,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(WHEEL0_3_GPIOx,WHEEL2_PIN,GPIO_PIN_RESET);
}

void follow_road(uint16_t *Data)
{
	while(1)
	{	
		if(HAL_GPIO_ReadPin(HAMPER2_GPIOx,HAMPER2_PIN) == RESET && HAL_GPIO_ReadPin(HAMPER3_GPIOx,HAMPER3_PIN) == SET)
		{
			go_right();
		}
		else if(HAL_GPIO_ReadPin(HAMPER3_GPIOx,HAMPER3_PIN) == RESET && HAL_GPIO_ReadPin(HAMPER2_GPIOx,HAMPER2_PIN) == SET)
		{
			go_left();
		}
		else
		{
			go_front();
		}
		
		if(*Data)
		{
			return;
		}
	}
}

void set_left_speed()
{
	if(__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_3))
	{
		speed_left>100?(speed_left -= 40):(speed_left = 500);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,speed_left);
	}
	else if(__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_4))
	{
		speed_left>100?(speed_left -= 40):(speed_left = 500);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,speed_left);
	}
}

void set_right_speed()
{
	if(__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_1))
	{
		speed_right>100?(speed_right -= 40):(speed_right = 500);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,speed_right);
	}
	else if(__HAL_TIM_GET_COMPARE(&htim3,TIM_CHANNEL_2))
	{
		speed_right>100?(speed_right -= 40):(speed_right = 500);
		__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,speed_right);
	}
}

void delay_us(uint32_t us) //利用CPU循环实现的非精准应用的微秒延时函数
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us); //使用HAL_RCC_GetHCLKFreq()函数获取主频值，经算法得到1微秒的循环次数
    while (delay--); //循环delay次，达到1微秒延时
}

double get_time_SR04(uint16_t *RxBuffer,uint8_t *flag_capture)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
	//输入捕获模式调整为上升沿触发
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    //启动定时器
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
	double distance = 0;
	double high_time = 0;
	uint32_t cnt= 0xffffff;
	while(cnt--)
	{
		if(*flag_capture == 2)
		{
			high_time = RxBuffer[2]- RxBuffer[1];    //高电平时间
			distance = (high_time / 1000000.0) * 340.0 / 2.0 * 100.0;
			if(distance <= 0)
			{
				distance = 0;
			}
			printf("buf[2]:%d buf[1]:%d  %.2f cm\n",RxBuffer[2],RxBuffer[1],distance);
			*flag_capture = 0;
			return distance;
		}
	}
	*flag_capture = 0;
	return 0;
}

//DHT11相关
void DHT11_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
 
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void DHT11_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
 
	GPIO_InitStruct.Pin  = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void DHT11_Strat(void)
{
	DHT11_OUT();   //PG9设置为输出模式
	DHT11_LOW;     //主机拉低总线
	HAL_Delay(20); //延迟必须大于18ms; 
	DHT11_HIGH;    //主机拉高总线等待DHT11响应
	delay_us(30);   
}

/**
  * @brief  DHT11发送响应信号
  * @param  无
  * @retval 返回值0/1  0：响应成功 1：响应失败
  */
uint8_t DHT11_Check(void)
{
	uint16_t retry = 0;
	DHT11_IN();
	//采用while循环的方式检测响应信号
	while(DHT11_IO_IN && retry <100) // DHT11会拉低 40us ~80us
	{
		retry++;
		delay_us(1);//1us
	}
	if(retry>=100) //判断当DHT11延迟超过80us时return 1 ， 说明响应失败
	{return  1;}
	else retry =  0;
		
	while(!DHT11_IO_IN && retry<100)// // DHT11拉低之后会拉高 40us ~80us
	{
		retry++;
		delay_us(1);//1us
	}
		
	if(retry>=100)
	{return 1;}
	return 0 ;
}

/**
  * @brief  DHT11读取一位数据
  * @param  无
  * @retval 返回值0/1  1：读取成功 0：读取失败
  */
uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0 ;
	while(DHT11_IO_IN && retry <100)//同上采用while循环的方式去采集数据
	{
		retry++;
		delay_us(1);
	}
	retry = 0 ;
	while(!DHT11_IO_IN && retry<100)
	{
		retry++;
		delay_us(1);
	}
 
	delay_us(40);              //结束信号，延时40us 
	if(DHT11_IO_IN) return 1;  //结束信号后，总线会被拉高 则返回1表示读取成功
	else 
	return 0 ;
}

/**
  * @brief  DHT11读取一个字节数据
  * @param  无
  * @retval 返回值：dat 将采集到的一个字节的数据返回
  */
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i , dat ;
	dat = 0 ;
	for(i=0; i<8; i++)
	{
		dat <<= 1; //通过左移存储数据
		dat |= DHT11_Read_Bit();
	}
	return dat ; 
}

/**
  * @brief  DHT11读取数据
  * @param  temp：温度值 humi ：湿度值
  * @retval 返回值0/1 0：读取数据成功 1：读取数据失败
  */
uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi)
{
	uint8_t buf[5];        //储存五位数据
    uint8_t i;    
	DHT11_Strat();         //起始信号
	if(DHT11_Check() == 0) //响应信号
    {
		for(i=0; i<5; i++)
		{
			buf[i] = DHT11_Read_Byte();
		}
		if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4]) //校验数据
		{
		    *humi = buf[0]; // 湿度
			*temp = buf[2]; // 温度
		}
	}else return 1;
	
   return 0 ;
}


void DHT11_test()
{
	DHT11_OUT();   //PG9设置为输出模式
	DHT11_LOW;     //主机拉低总线
	HAL_Delay(20); //延迟必须大于18ms; 
	DHT11_HIGH;    //主机拉高总线等待DHT11响应
	delay_us(30);
	DHT11_IN();
	while(1)
	{
		if(!DHT11_IO_IN)
		{
			PFout(9) ^= 1;
			return ;
		}
	}
}


