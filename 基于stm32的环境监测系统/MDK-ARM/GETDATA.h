#ifndef __GETDATA_H__
#define __GETDATA_H__

#include "sys.h"

#define DHT11_LOW (PCout(6) = 0)
#define	DHT11_HIGH (PCout(6) = 1)
#define DHT11_IO_IN (PCin(6))

void DHT11_OUT(void);

void DHT11_IN(void);

void DHT11_Strat(void);

uint8_t DHT11_Check(void);

uint8_t DHT11_Read_Bit(void);

uint8_t DHT11_Read_Byte(void);

uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi);

void MQ_2_Data_start_add(void);

void MQ_2_Data_start_decrese(void);

void temperature_start_add(void);

void temperature_start_decrese(void);

void humidity_start_add(void);

void humidity_start_decrese(void);

#endif

