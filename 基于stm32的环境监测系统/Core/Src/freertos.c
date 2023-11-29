/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "getdata.h"
#include "oled.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint16_t MQ_2_Data_start = 0;	//烟雾浓度校正值
uint8_t temperature_start = 0;//温度校正值
uint8_t humidity_start = 0;//湿度校正值
uint16_t MQ_2_Data = 0;//烟雾浓度
uint8_t temperature = 0;//温度
uint8_t humidity = 0;//湿度


/* USER CODE END Variables */
osThreadId GETDATAHandle;
osThreadId SHOWDATAHandle;
osThreadId LET_LED_ONHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of GETDATA */
  osThreadDef(GETDATA, StartDefaultTask, osPriorityNormal, 0, 128);
  GETDATAHandle = osThreadCreate(osThread(GETDATA), NULL);

  /* definition and creation of SHOWDATA */
  osThreadDef(SHOWDATA, StartTask02, osPriorityNormal, 0, 128);
  SHOWDATAHandle = osThreadCreate(osThread(SHOWDATA), NULL);

  /* definition and creation of LET_LED_ON */
  osThreadDef(LET_LED_ON, StartTask03, osPriorityNormal, 0, 128);
  LET_LED_ONHandle = osThreadCreate(osThread(LET_LED_ON), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the GETDATA thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  //osDelay(1);
	while(1)
	{
		DHT11_Read_Data(&temperature , &humidity);
		MQ_2_Data = Get_Adc(MQ_2_Data);
		osDelay(1000);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the SHOWDATA thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
	OLED_P6x8Str(1,1,"smoke: ");
	OLED_P6x8Str(1,2,"tem: ");
	OLED_P6x8Str(1,3,"hum: ");
  while(1)
	{
		OLED_P6x8Num(45,1,MQ_2_Data + MQ_2_Data_start,4);
		OLED_P6x8Num(32,2,temperature + temperature_start,4);
		OLED_P6x8Num(32,3,humidity + humidity_start,4);
		osDelay(1000);
	}
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the LET_LED_ON thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_8) == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
		}
		else 
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
		}
		osDelay(1000);
	}
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void MQ_2_Data_start_add()
{
	MQ_2_Data_start+=1;
}

void MQ_2_Data_start_decrese()
{
	MQ_2_Data_start-=1;
}

void temperature_start_add()
{
	temperature_start+=1;
}

void temperature_start_decrese()
{
	temperature_start-=1;
}

void humidity_start_add()
{
	humidity_start+=1;
}

void humidity_start_decrese()
{
	humidity_start-=1;
}
/* USER CODE END Application */
