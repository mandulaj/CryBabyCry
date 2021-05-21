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


#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "mfs_model.h"
#include "mfs_model_data.h"

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

#ifdef MFCC_FLOAT
static float32_t MFCC_Buffer1[MFCC_LENGTH][N_CEPS];
static float32_t MFCC_Buffer2[MFCC_LENGTH][N_CEPS];

#elif defined(MFCC_Q15)
static q15_t MFCC_Buffer1[MFCC_LENGTH][N_CEPS];
static q15_t MFCC_Buffer2[MFCC_LENGTH][N_CEPS];

#endif
static struct CPB cpb_struct;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1000 * 4,
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


float32_t in[128];
float32_t out[256];
float32_t temp[128];



/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/*for(int i =0; i < 64; i++){
		in[i] = i;
	}

	dct2_64_f32(temp, in, out);

	for(int i =0; i < 64; i+=4){
		USART1_printf("%f %f %f %f\n", out[i],out[i+1],out[i+2],out[i+3]);
	}

	USART1_printf("\n");

	  while(1);*/

	// Start the Microphone Filtering
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, REC_BUF_LENGTH)){
	  Error_Handler();
	}

	uint32_t ulNotifiedValue;
	while(1){
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);

		printf("Stack water marks:\n");
		printf("audio_preprocessing Used Stack: %ld Bytes of %ld Bytes\n", audio_preproces_attributes.stack_size - uxTaskGetStackHighWaterMark( audio_preprocesHandle)*4,audio_preproces_attributes.stack_size);
		printf("nn_inference:       Used Stack: %ld Bytes of %ld Bytes\n", nn_inference_attributes.stack_size - uxTaskGetStackHighWaterMark( nn_inferenceHandle )*4, nn_inference_attributes.stack_size);

//		USART1_printf("%f\n", RecBuff[0]/0.3);
		printf("default:            Used Stack:  %ld Bytes of %ld Bytes\n", defaultTask_attributes.stack_size- uxTaskGetStackHighWaterMark( NULL )*4, defaultTask_attributes.stack_size);


		osDelay(1);
	}

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_vTask_audio_preproces */
volatile int proc = 1;
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
	float32_t *pMFCC_Buff = (float32_t*)MFCC_Buffer1;


	for(;;)
	{
		//	uint16_t new_input_buf = circular_proc_buffer->full_frame;
		// Wait for the IRS to trigger the processing
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);

		if(proc == 0)
			continue;

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
		cycles[MFCC_index-1] = getCycles();


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
/*
	ai_error ai_err;
	ai_i32 nbatch;

	AI_ALIGNED(4) ai_u8 activations[AI_MFS_MODEL_DATA_ACTIVATIONS_SIZE];

	AI_ALIGNED(4) ai_i8 in_data[AI_MFS_MODEL_IN_1_SIZE_BYTES];
	AI_ALIGNED(4) ai_i8 out_data[AI_MFS_MODEL_OUT_1_SIZE_BYTES];

	ai_handle mfs_model = AI_HANDLE_NULL;

	ai_buffer ai_input[AI_MFS_MODEL_IN_NUM] = AI_MFS_MODEL_IN;
	ai_buffer ai_output[AI_MFS_MODEL_OUT_NUM] = AI_MFS_MODEL_OUT;

	ai_network_params ai_params = {
			AI_MFS_MODEL_DATA_WEIGHTS(ai_mfs_model_data_weights_get()),
			AI_MFS_MODEL_DATA_ACTIVATIONS(activations)
	};

	ai_input[0].n_batches = 1;
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].n_batches = 1;
	ai_output[0].data = AI_HANDLE_PTR(out_data);

	ai_err = ai_mfs_model_create(&mfs_model, AI_MFS_MODEL_DATA_CONFIG);
	if(ai_err.type != AI_ERROR_NONE){
		USART1_printf("Error, could not create NN instance\n");
		vTaskSuspend(NULL);
	}

	if(!ai_mfs_model_init(mfs_model, &ai_params)){
		USART1_printf("Error, could not init NN\n");
		vTaskSuspend(NULL);
	}

	HAL_TIM_Base_Start(&htim16);
	uint32_t timestamp = htim16.Instance->CNT;


	nbatch  = ai_mfs_model_run(mfs_model, ai_input, ai_output);
	if (nbatch != 1) {
		USART1_printf("Error, could not run NN inference\n");
		vTaskSuspend(NULL);
	}

	uint8_t cry,other;

	cry = out_data[0];
	other = out_data[1];

	USART1_printf("Detected: %s   cry: %d other: %d Inference Time (us): %d\n", (cry>other)? "cry" : "other", cry, other, htim16.Instance->CNT - timestamp);
*/
	printf("Start Free Stack nn_inf: %ld", uxTaskGetStackHighWaterMark(NULL));

	/* Infinite loop */
	for(;;)
	{
		xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
		vTaskSuspend(audio_preprocesHandle);

		pMFCC_Buff = (float32_t *)ulNotifiedValue;

		buffer_counter++;
		// Swap the buffer pointers




		if(buffer_counter >= 1){
			printf("Printing buffer %p\n", pMFCC_Buff);
			printf("Cycle counts: \n");
			for(int i = 0; i < MFCC_LENGTH; i++){
				printf("%ld ", cycles[i]);
			}
			printf("\n\nBuffer 1:\n");

			for(int i = 0; i< MFCC_LENGTH; i++){
				for(int m = 0; m < N_CEPS; m+=6){
#ifdef MFCC_FLOAT
					printf("%f %f %f %f %f %f ",
												MFCC_Buffer1[i][m],
												MFCC_Buffer1[i][m+1],
												MFCC_Buffer1[i][m+2],
												MFCC_Buffer1[i][m+3],
												MFCC_Buffer1[i][m+4],
												MFCC_Buffer1[i][m+5]);

#elif defined(MFCC_Q15)
					USART1_printf("%i ", MFCC_Buffer1[i][m]);
#endif

				}
				printf("\n");

			}
//			USART1_printf("\n\nBuffer 2:\n");
//			for(int i = 0; i< MFCC_LENGTH; i++){
//				for(int m = 0; m < N_CEPS; m++){
//#ifdef MFCC_FLOAT
//					USART1_printf("%f ", MFCC_Buffer2[i][m]);
//
//#elif defined(MFCC_Q15)
//					USART1_printf("%i ", MFCC_Buffer2[i][m]);
//#endif
//
//				}
//				USART1_printf("\n");
//
//			}


			xTaskNotify(defaultTaskHandle, 0, eSetValueWithOverwrite);
			printf("End Free Stack nn_inf: %ld", uxTaskGetStackHighWaterMark(NULL));


//			vTaskSuspend(NULL);
		}
	}
  /* USER CODE END vTask_nn_inference */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
