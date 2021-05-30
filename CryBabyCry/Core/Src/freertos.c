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
#include "tim.h"


#include "arm_math.h"

#include "ai.h"

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


static uint32_t cycles[MFCC_LENGTH/2];


float32_t MFCC_Buffer1[MFCC_LENGTH][N_CEPS];
float32_t MFCC_Buffer2[MFCC_LENGTH][N_CEPS];


static struct CPB cpb_struct;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for audio_preproces */
osThreadId_t audio_preprocesHandle;
const osThreadAttr_t audio_preproces_attributes = {
  .name = "audio_preproces",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};
/* Definitions for nn_inference */
osThreadId_t nn_inferenceHandle;
const osThreadAttr_t nn_inference_attributes = {
  .name = "nn_inference",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void vTask_audio_preproces(void *argument);
void vTask_nn_inference(void *argument);

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

	// Set pointer to first MFCC buffer
	//pMFCC_Buff = (float32_t*)MFCC_Buffer1;


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

  /* creation of nn_inference */
  nn_inferenceHandle = osThreadNew(vTask_nn_inference, NULL, &nn_inference_attributes);

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
	// Start the Microphone Filtering

#if(CURRENT_MODE == MODE_VALIDATION)

	Validation_Start();

#elif(CURRENT_MODE == MODE_DEMO)
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, REC_BUF_LENGTH)){
		  Error_Handler();
	}
#endif



	uint32_t ulNotifiedValue;
	while(1){
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);

		printf("Stack water marks:\n");
		printf("audio_preprocessing Used Stack: %ld Bytes of %ld Bytes\n", audio_preproces_attributes.stack_size - uxTaskGetStackHighWaterMark( audio_preprocesHandle)*4,audio_preproces_attributes.stack_size);
		printf("nn_inference:       Used Stack: %ld Bytes of %ld Bytes\n", nn_inference_attributes.stack_size - uxTaskGetStackHighWaterMark( nn_inferenceHandle )*4, nn_inference_attributes.stack_size);

//		USART1_printf("%f\n", RecBuff[0]/0.3);
		printf("default:            Used Stack:  %ld Bytes of %ld Bytes\n", defaultTask_attributes.stack_size- uxTaskGetStackHighWaterMark( NULL )*4, defaultTask_attributes.stack_size);

		printf("minimumFreeHeap %d\n", xPortGetMinimumEverFreeHeapSize());
		osDelay(1);
	}

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_vTask_audio_preproces */
/**
* @brief Function implementing the ae
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
	float32_t *pMFCC_Buff = (float32_t*)MFCC_Buffer1;


	for(;;)
	{
		//	uint16_t new_input_buf = circular_proc_buffer->full_frame;
		// Wait for the IRS to trigger the processing
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);


		//printf("Task Started: %lx\n", ulNotifiedValue);
		ResetTimer();
		StartTimer();

		int32_t *pRecBuff;
		if(ulNotifiedValue == 1){
			pRecBuff = RecBuff;
		} else if(ulNotifiedValue == 2){
			pRecBuff = RecBuff + FRAME_LENGTH;
		} else {
			continue;
		}

		// Read new data into
		for(uint32_t i = 0; i < FRAME_LENGTH; i++){
			cpb->full_frame[i] = (q15_t)(pRecBuff[i] >> 8);
		}


		// Process previous half plus new half
		// 72778 Cycles -Ofast 85543 cycles -Og
		MFCC_Process_Frame(cpb->half_frame, &pMFCC_Buff[(MFCC_index++) * N_CEPS]);
		MFCC_Process_Frame(cpb->full_frame, &pMFCC_Buff[(MFCC_index++) * N_CEPS]);

		// Swap buffers
		CPB_copyFull(cpb);
		StopTimer();
		cycles[(MFCC_index-1)>>1U] = getCycles();


		// MFCC Buffer is full
		if(MFCC_index >= MFCC_LENGTH){
			MFCC_index = 0;

			// Swap the buffer pointer
			if((void*)pMFCC_Buff == (void*)MFCC_Buffer1){
				pMFCC_Buff = (float32_t*)MFCC_Buffer2;
				// Notify nn_inference that we just finished with Buffer1
				xTaskNotify(nn_inferenceHandle,(uint32_t) MFCC_Buffer1, eSetValueWithOverwrite);
			} else {
				pMFCC_Buff = (float32_t*)MFCC_Buffer1;
				// Notify nn_inference that we just finished with Buffer2
				xTaskNotify(nn_inferenceHandle, (uint32_t) MFCC_Buffer2, eSetValueWithOverwrite);
			}
		}
	}
  /* USER CODE END vTask_audio_preproces */
}

/* USER CODE BEGIN Header_vTask_nn_inference */








/**
* @brief Function implementing the nn_inference thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTask_nn_inference */
void vTask_nn_inference(void *argument)
{
  /* USER CODE BEGIN vTask_nn_inference */
  /* Infinite loop */
	uint32_t ulNotifiedValue;
	float32_t *pMFCC_Buff;
	uint32_t buffer_counter = 0;
	ai_i8 *nn_input_buffer;
	ai_i8 *nn_output_buffer;
	int8_t cry,other;

	uint32_t timestamp;



	AI_NN_init();

	// Get input and output buffer pointers
	AI_get_buffers(&nn_input_buffer, &nn_output_buffer);


	printf("Start Free Stack nn_inf: %ld\n", uxTaskGetStackHighWaterMark(NULL));

	/* Infinite loop */
	for(;;)
	{
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
//		vTaskSuspend(audio_preprocesHandle);

		pMFCC_Buff = (float32_t *)ulNotifiedValue;

		buffer_counter++;
		// Swap the buffer pointers





		//151365 cycles
		//arm_offset_f32(pMFCC_Buff, -mean, pMFCC_Buff, MFCC_LENGTH*N_CEPS);
		//arm_scale_f32(pMFCC_Buff, 1.0f/std, pMFCC_Buff, MFCC_LENGTH*N_CEPS);

		// Subtract the mean and divide by std
		// combined with divide by scale and add zero point


		//offset = AI_INPUT_ZERO_POINT * AI_INPUT_SCALE * std - mean;
		//scale = AI_INPUT_SCALE * std * 128.0f; // Divide Factor of 128 because arm_float_to_q7 multiplies by 128


		AI_quantize(pMFCC_Buff, nn_input_buffer, MFCC_LENGTH*N_CEPS);



		HAL_TIM_Base_Start(&htim16);
		htim16.Instance->CNT = 0;
		timestamp = htim16.Instance->CNT;

		AI_NN_inference(nn_input_buffer, nn_output_buffer);



		cry = nn_output_buffer[0];
		other = nn_output_buffer[1];
		printf("================\n");
		printf("Detected: %s\n",(cry>other)? "CRY" : "OTHER");
		printf("================\n");
		printf("cry: %d other: %d Inference Time (ms): %f\n", cry, other, (htim16.Instance->CNT - timestamp)/100.0f);

		if (cry > other){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
		} else {
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
		}
		print_buffer_q31(cycles, 32);
	}
  /* USER CODE END vTask_nn_inference */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
