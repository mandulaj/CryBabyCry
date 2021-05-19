/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

#include "MFCC.h"
#include "helper.h"
#include "config.h"
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include "CycleCounter.h"
#include "dfsdm.h"
#include "dma.h"

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




extern int32_t RecBuff[REC_BUF_LENGTH];


static uint32_t cycles[MFCC_LENGTH];
static float32_t MFCC_OUT[MFCC_LENGTH][N_FILTS];
static struct CPB cpb_struct;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for audio_preproces */
osThreadId_t audio_preprocesHandle;
const osThreadAttr_t audio_preproces_attributes = {
  .name = "audio_preproces",
  .stack_size = 400 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void vTask_audio_preproces(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	// Initialize circular processing buffer
	CPB_Init(&cpb_struct);
	MFCC_Init();

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of audio_preproces */
  audio_preprocesHandle = osThreadNew(vTask_audio_preproces, NULL, &audio_preproces_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint32_t ulNotifiedValue;

	// Start the Microphone Filtering
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, REC_BUF_LENGTH)){
	  Error_Handler();
	}


	/* Infinite loop */
	for(;;)
	{
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
		vTaskSuspend(audio_preprocesHandle);

		printf("Main\n");


		for(int i = 0; i < MFCC_LENGTH; i++){
			printf("%ld ", cycles[i]);
		}
		printf("\n\n");


		for(int i = 0; i< MFCC_LENGTH; i++){
			for(int m = 0; m < N_FILTS; m++){
				printf("%f ", MFCC_OUT[i][m]);

			}
			printf("\n");
		}

		vTaskSuspend(NULL);

	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_vTask_audio_preproces */
/**
* @brief Function implementing the audio_preproces thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTask_audio_preproces */
void vTask_audio_preproces(void *argument)
{
  /* USER CODE BEGIN vTask_audio_preproces */
  /* Infinite loop */


	uint32_t ulNotifiedValue;
	struct CPB *cpb = &cpb_struct;
	uint32_t MFCC_index = 0;


	for(;;)
	{
		//	uint16_t new_input_buf = circular_proc_buffer->full_frame;
		// Wait for the IRS to trigger the processing
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);

		printf("Task Started: %lx\n", ulNotifiedValue);
		ResetTimer();
		StartTimer();


		if(ulNotifiedValue == 1) {
			// Copy Lower half of REC_BUF
			for(uint32_t i = 0; i < REC_BUF_LENGTH/2; i++){
				cpb->full_frame[i] = (q15_t)(RecBuff[i] >> 8);
			}
		} else if(ulNotifiedValue == 2){
			// Copy Upper half of REC_BUF
			for(uint32_t i = 0; i < REC_BUF_LENGTH/2; i++){
				cpb->full_frame[i] = (q15_t)(RecBuff[i+REC_BUF_LENGTH/2] >> 8);
			}
		} else {
			// Need to throw error
			continue;
		}

		// Process previous half plus new half
		// 72778 Cycles -Ofast 85543 cycles -Og

		MFCC_Process_Frame(cpb->half_frame, MFCC_OUT[MFCC_index++]);
		MFCC_Process_Frame(cpb->full_frame, MFCC_OUT[MFCC_index++]);

		CPB_copyFull(cpb);
		StopTimer();
		cycles[MFCC_index-1] = getCycles();


		// MFCC Buffer is full
		if(MFCC_index >= MFCC_LENGTH){
			MFCC_index = 0;
			xTaskNotify(defaultTaskHandle, 0, eSetValueWithOverwrite);
		}
	}
  /* USER CODE END vTask_audio_preproces */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
