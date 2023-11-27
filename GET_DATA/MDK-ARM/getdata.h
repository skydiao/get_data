#ifndef __GETDATA_H__
#define __GETDATA_H__

#include "sys.h"

#define WHEEL0_PIN GPIO_PIN_6 //A A
#define WHEEL1_PIN GPIO_PIN_7 //A B
#define WHEEL2_PIN GPIO_PIN_8 //B A
#define WHEEL3_PIN GPIO_PIN_9 //B B

#define WHEEL0_3_CLOCK RCC_AHB1Periph_GPIOC

#define WHEEL0_3_GPIOx GPIOC

#define HAMPER2_PIN GPIO_PIN_6
#define HAMPER3_PIN GPIO_PIN_7

#define HAMPER2_CLOCK RCC_AHB1Periph_GPIOD
#define HAMPER3_CLOCK RCC_AHB1Periph_GPIOB

#define HAMPER2_GPIOx GPIOD
#define HAMPER3_GPIOx GPIOB

#define DHT11_LOW (PGout(9) = 0)
#define	DHT11_HIGH (PGout(9) = 1)
#define DHT11_IO_IN (PGin(9))

void set_left_speed(void);

void set_right_speed(void);

void car_wheel_init(void);

void hamper_init(void);

void go_front(void);

void go_back(void);

void go_left(void);

void go_right(void);

void car_stop(void);

void follow_road(uint16_t *Data);

double get_time_SR04(uint16_t *RxBuffer,uint8_t *flag_capture);

void delay_us(uint32_t us);

void DHT11_OUT(void);

void DHT11_IN(void);

void DHT11_Strat(void);

uint8_t DHT11_Check(void);

uint8_t DHT11_Read_Bit(void);

uint8_t DHT11_Read_Byte(void);

uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi);

void DHT11_test(void);

#endif

