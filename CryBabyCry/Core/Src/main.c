/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ARM_MATH_CM4

#include "arm_math.h"

#include "CycleCounter.h"
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
DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;
DMA_HandleTypeDef hdma_dfsdm1_flt0;

USART_HandleTypeDef husart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_Init(void);
static void MX_DFSDM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


volatile bool firstHalfFull = false;
volatile bool secondHalfFull = false;


volatile bool filling_disabled = false;

volatile uint32_t buffer_index = 0;
volatile bool filling_buf1 = true;

#define QUEUELENGTH 2048 //16384*2
#define PROCESSED_BUF 16384
#define FFT_LEN 512

int32_t RecBuff[QUEUELENGTH];

int16_t ProcessedBuff1[PROCESSED_BUF];
int16_t ProcessedBuff2[PROCESSED_BUF];
int16_t ProcessedBuff3[PROCESSED_BUF/2];

int16_t amplitude;


#define cyclesDebugLength 100

int32_t cyclesDebug[cyclesDebugLength];
int32_t cycles_index = 0;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small msg_info (option LD Linker->Libraries->Small msg_info
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the serial port and Loop until the end of transmission */
  while (HAL_OK != HAL_USART_Transmit(&husart1, (uint8_t *) &ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
  uint8_t ch = 0;
  while (HAL_OK != HAL_USART_Receive(&husart1, (uint8_t *)&ch, 1, 30000))
  {
    ;
  }
  return ch;
}




const int16_t qhamming_512[] = {
		0x0a3d, 0x0a3e, 0x0a41, 0x0a47, 0x0a4f, 0x0a59, 0x0a66, 0x0a75,
		0x0a86, 0x0a99, 0x0aaf, 0x0ac7, 0x0ae1, 0x0afd, 0x0b1c, 0x0b3d,
		0x0b60, 0x0b85, 0x0bad, 0x0bd6, 0x0c02, 0x0c31, 0x0c61, 0x0c94,
		0x0cc8, 0x0cff, 0x0d39, 0x0d74, 0x0db1, 0x0df1, 0x0e33, 0x0e77,
		0x0ebd, 0x0f05, 0x0f4f, 0x0f9b, 0x0fea, 0x103a, 0x108c, 0x10e1,
		0x1137, 0x1190, 0x11eb, 0x1247, 0x12a5, 0x1306, 0x1368, 0x13cd,
		0x1433, 0x149b, 0x1505, 0x1571, 0x15de, 0x164e, 0x16bf, 0x1732,
		0x17a7, 0x181e, 0x1896, 0x1910, 0x198c, 0x1a0a, 0x1a89, 0x1b0a,
		0x1b8c, 0x1c10, 0x1c96, 0x1d1d, 0x1da6, 0x1e30, 0x1ebc, 0x1f49,
		0x1fd8, 0x2068, 0x20fa, 0x218d, 0x2221, 0x22b7, 0x234e, 0x23e6,
		0x2480, 0x251b, 0x25b7, 0x2654, 0x26f3, 0x2793, 0x2833, 0x28d5,
		0x2979, 0x2a1d, 0x2ac2, 0x2b68, 0x2c0f, 0x2cb8, 0x2d61, 0x2e0b,
		0x2eb6, 0x2f61, 0x300e, 0x30bc, 0x316a, 0x3219, 0x32c9, 0x3379,
		0x342a, 0x34dc, 0x358f, 0x3642, 0x36f5, 0x37a9, 0x385e, 0x3913,
		0x39c9, 0x3a7f, 0x3b35, 0x3bec, 0x3ca4, 0x3d5b, 0x3e13, 0x3ecb,
		0x3f84, 0x403c, 0x40f5, 0x41ae, 0x4267, 0x4320, 0x43d9, 0x4493,
		0x454c, 0x4605, 0x46bf, 0x4778, 0x4831, 0x48ea, 0x49a3, 0x4a5c,
		0x4b14, 0x4bcc, 0x4c84, 0x4d3c, 0x4df3, 0x4eaa, 0x4f61, 0x5017,
		0x50cd, 0x5183, 0x5238, 0x52ec, 0x53a0, 0x5453, 0x5506, 0x55b8,
		0x566a, 0x571b, 0x57cb, 0x587a, 0x5929, 0x59d7, 0x5a84, 0x5b30,
		0x5bdb, 0x5c86, 0x5d2f, 0x5dd8, 0x5e80, 0x5f26, 0x5fcc, 0x6071,
		0x6115, 0x61b7, 0x6259, 0x62f9, 0x6398, 0x6436, 0x64d3, 0x656e,
		0x6609, 0x66a2, 0x6739, 0x67d0, 0x6865, 0x68f8, 0x698b, 0x6a1c,
		0x6aab, 0x6b39, 0x6bc6, 0x6c51, 0x6cda, 0x6d62, 0x6de9, 0x6e6e,
		0x6ef1, 0x6f72, 0x6ff2, 0x7071, 0x70ed, 0x7168, 0x71e2, 0x7259,
		0x72cf, 0x7343, 0x73b5, 0x7426, 0x7494, 0x7501, 0x756c, 0x75d5,
		0x763c, 0x76a1, 0x7705, 0x7766, 0x77c5, 0x7823, 0x787e, 0x78d8,
		0x792f, 0x7985, 0x79d8, 0x7a2a, 0x7a79, 0x7ac7, 0x7b12, 0x7b5b,
		0x7ba2, 0x7be7, 0x7c2a, 0x7c6a, 0x7ca9, 0x7ce5, 0x7d20, 0x7d58,
		0x7d8e, 0x7dc1, 0x7df3, 0x7e22, 0x7e4f, 0x7e7a, 0x7ea3, 0x7ec9,
		0x7eee, 0x7f10, 0x7f2f, 0x7f4d, 0x7f68, 0x7f81, 0x7f98, 0x7fac,
		0x7fbe, 0x7fce, 0x7fdc, 0x7fe7, 0x7ff1, 0x7ff7, 0x7ffc, 0x7ffe,
		0x7ffe, 0x7ffc, 0x7ff7, 0x7ff1, 0x7fe7, 0x7fdc, 0x7fce, 0x7fbe,
		0x7fac, 0x7f98, 0x7f81, 0x7f68, 0x7f4d, 0x7f2f, 0x7f10, 0x7eee,
		0x7ec9, 0x7ea3, 0x7e7a, 0x7e4f, 0x7e22, 0x7df3, 0x7dc1, 0x7d8e,
		0x7d58, 0x7d20, 0x7ce5, 0x7ca9, 0x7c6a, 0x7c2a, 0x7be7, 0x7ba2,
		0x7b5b, 0x7b12, 0x7ac7, 0x7a79, 0x7a2a, 0x79d8, 0x7985, 0x792f,
		0x78d8, 0x787e, 0x7823, 0x77c5, 0x7766, 0x7705, 0x76a1, 0x763c,
		0x75d5, 0x756c, 0x7501, 0x7494, 0x7426, 0x73b5, 0x7343, 0x72cf,
		0x7259, 0x71e2, 0x7168, 0x70ed, 0x7071, 0x6ff2, 0x6f72, 0x6ef1,
		0x6e6e, 0x6de9, 0x6d62, 0x6cda, 0x6c51, 0x6bc6, 0x6b39, 0x6aab,
		0x6a1c, 0x698b, 0x68f8, 0x6865, 0x67d0, 0x6739, 0x66a2, 0x6609,
		0x656e, 0x64d3, 0x6436, 0x6398, 0x62f9, 0x6259, 0x61b7, 0x6115,
		0x6071, 0x5fcc, 0x5f26, 0x5e80, 0x5dd8, 0x5d2f, 0x5c86, 0x5bdb,
		0x5b30, 0x5a84, 0x59d7, 0x5929, 0x587a, 0x57cb, 0x571b, 0x566a,
		0x55b8, 0x5506, 0x5453, 0x53a0, 0x52ec, 0x5238, 0x5183, 0x50cd,
		0x5017, 0x4f61, 0x4eaa, 0x4df3, 0x4d3c, 0x4c84, 0x4bcc, 0x4b14,
		0x4a5c, 0x49a3, 0x48ea, 0x4831, 0x4778, 0x46bf, 0x4605, 0x454c,
		0x4493, 0x43d9, 0x4320, 0x4267, 0x41ae, 0x40f5, 0x403c, 0x3f84,
		0x3ecb, 0x3e13, 0x3d5b, 0x3ca4, 0x3bec, 0x3b35, 0x3a7f, 0x39c9,
		0x3913, 0x385e, 0x37a9, 0x36f5, 0x3642, 0x358f, 0x34dc, 0x342a,
		0x3379, 0x32c9, 0x3219, 0x316a, 0x30bc, 0x300e, 0x2f61, 0x2eb6,
		0x2e0b, 0x2d61, 0x2cb8, 0x2c0f, 0x2b68, 0x2ac2, 0x2a1d, 0x2979,
		0x28d5, 0x2833, 0x2793, 0x26f3, 0x2654, 0x25b7, 0x251b, 0x2480,
		0x23e6, 0x234e, 0x22b7, 0x2221, 0x218d, 0x20fa, 0x2068, 0x1fd8,
		0x1f49, 0x1ebc, 0x1e30, 0x1da6, 0x1d1d, 0x1c96, 0x1c10, 0x1b8c,
		0x1b0a, 0x1a89, 0x1a0a, 0x198c, 0x1910, 0x1896, 0x181e, 0x17a7,
		0x1732, 0x16bf, 0x164e, 0x15de, 0x1571, 0x1505, 0x149b, 0x1433,
		0x13cd, 0x1368, 0x1306, 0x12a5, 0x1247, 0x11eb, 0x1190, 0x1137,
		0x10e1, 0x108c, 0x103a, 0x0fea, 0x0f9b, 0x0f4f, 0x0f05, 0x0ebd,
		0x0e77, 0x0e33, 0x0df1, 0x0db1, 0x0d74, 0x0d39, 0x0cff, 0x0cc8,
		0x0c94, 0x0c61, 0x0c31, 0x0c02, 0x0bd6, 0x0bad, 0x0b85, 0x0b60,
		0x0b3d, 0x0b1c, 0x0afd, 0x0ae1, 0x0ac7, 0x0aaf, 0x0a99, 0x0a86,
		0x0a75, 0x0a66, 0x0a59, 0x0a4f, 0x0a47, 0x0a41, 0x0a3e, 0x0a3d
};









#define FRAME_LENGTH 400


struct CPB{
	// Contiguous memory for internal memory buffer
	int16_t _internalBuffer[FRAME_LENGTH + (FRAME_LENGTH/2)];
	int16_t *full_frame;
	int16_t *half_frame;

};


void CPB_Init(struct CPB *cpb){
	cpb->full_frame = cpb->_internalBuffer + (FRAME_LENGTH/2);
	cpb->half_frame = cpb->_internalBuffer;
}


void CPB_copyFull(struct CPB *cpb){
	memcpy(cpb->half_frame, cpb->full_frame + (FRAME_LENGTH/2), FRAME_LENGTH/2 * sizeof(uint16_t));
}


struct CPB circular_proc_buffer;





volatile int ready = 0;







void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{

	//uint16_t buf = circular_proc_buffer->full_frame;

	for(uint32_t i = 0; i < QUEUELENGTH/2; i++){
		ProcessedBuff1[i] = (int16_t)(RecBuff[i]>>8);
	}


	firstHalfFull = true;
	ready++;


	//PROCESS(circular_proc_buffer->half_frame);

	//PROCESS(circular_proc_buffer->full_frame);


	// Copy end of full_frame to the half frame
	//CPB_copyFull(&circular_proc_buffer);
/*

	if(filling_disabled)
		return;

	if(filling_buf1) {
		for(uint32_t i = 0; i < QUEUELENGTH/2; i++) {
			ProcessedBuff1[buffer_index++] = (int16_t)(RecBuff[i]>>8);
		}
	} else {
		for(uint32_t i = 0; i < QUEUELENGTH/2; i++) {

			ProcessedBuff2[buffer_index++] = (int16_t)(RecBuff[i]>>8);
		}
	}*/
}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{


	for(uint32_t i = QUEUELENGTH/2; i < QUEUELENGTH; i++){
			ProcessedBuff2[i-QUEUELENGTH/2] = (int16_t)(RecBuff[i]>>8);
	}
	/*

	ResetTimer();
	StartTimer();
	if(filling_disabled)
			return;

	if(filling_buf1) {
		for(uint32_t i = QUEUELENGTH/2; i < QUEUELENGTH; i++) {
			ProcessedBuff1[buffer_index++] = (int16_t)(RecBuff[i]>>8);
		}
	} else {
		for(uint32_t i = QUEUELENGTH/2; i < QUEUELENGTH; i++){

			ProcessedBuff2[buffer_index++] = (int16_t)(RecBuff[i]>>8);
		}
	}

	if(buffer_index >= PROCESSED_BUF){
		if(filling_buf1){
			firstHalfFull = true;
		} else {
			secondHalfFull = true;
		}
		filling_buf1= !filling_buf1;
		buffer_index = 0;
	}
	StopTimer();
	if(cycles_index > cyclesDebugLength){
		cycles_index = 0;
	}
	cyclesDebug[cycles_index++] = getCycles();*/
}


int16_t buffer[FFT_LEN];
int16_t bufferout[FFT_LEN];
int16_t bufferout2[FFT_LEN];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  // Initialize circular processing buffer
  CPB_Init(&circular_proc_buffer);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_Init();
  MX_DFSDM1_Init();
  /* USER CODE BEGIN 2 */


  if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, RecBuff, QUEUELENGTH)){
	  Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  if(firstHalfFull){
//		  int32_t max = -34000;
//		  for (int i = 0; i < QUEUELENGTH/2; i++){
//			  int32_t t = ProcessedBuff1[i];
//			  if(t > max){
//				  max = t;
//			  }
//		  }
//		  printf("%d\n", max);
//		  firstHalfFull = true;
//	  }

/*
	if(firstHalfFull){
		printf("First Half:\n");
		for(int i=0;i<PROCESSED_BUF;i++){
			printf("%i ",ProcessedBuff1[i]);
		}
		firstHalfFull = false;
	}
*/

	  if(ready>50){
		  ResetTimer();

		  StartTimer();



		  memcpy(buffer, ProcessedBuff1, FFT_LEN*sizeof(uint16_t));


		  printf("Raw input\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", buffer[i]);
		  }
		  printf("Done\n\n");
		  /*printf("Input\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", buffer[i]);
		  }
		  printf("Done\n\n");*/


		  // Amplify signal by 8
		  arm_shift_q15(buffer, 3, buffer, FFT_LEN);

		  printf("Amplified input\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", buffer[i]);
		  }
		  printf("Done\n\n");

		  // Hamming scaling
		  arm_mult_q15(buffer, qhamming_512, buffer, FFT_LEN);

		  printf("Scaled input\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", buffer[i]);
		  }
		  printf("Done\n\n");

		  // FFT
		  arm_rfft_instance_q15  fft_inst;
		  arm_rfft_init_q15(&fft_inst, FFT_LEN, 0, 0);
		  arm_rfft_q15(&fft_inst, buffer, bufferout);

		  printf("Raw FFT\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", bufferout[i]);
		  }
		  printf("Done\n\n");
		  // Shift FFT to avoid loosing lower bits
		  arm_shift_q15(bufferout, 6, bufferout, FFT_LEN);

		  printf("Shifted FFT\n");
		  for(int i = 0; i < FFT_LEN; i++){
			  printf("%i ", bufferout[i]);
		  }
		  printf("Done\n\n");
		  // Compute magnitude squared (faster than just magnitude)
		  arm_cmplx_mag_squared_q15(bufferout, bufferout, FFT_LEN/2);
		  StopTimer();

		  printf("Cycles: %d\n", getCycles());


		  printf("Output\n");
		  for(int i = 0; i < FFT_LEN/2; i++){
			  printf("%i ", bufferout[i]);
		  }
		  printf("Done\n\n");


/*
		  printf("Output mag squared\n");
		  for(int i = 0; i < FFT_LEN/2; i++){
			  printf("%i ", bufferout[i]);
		  }
		  printf("Done\n\n");*/



		  while(1){}
	  }

	  /*
	if(secondHalfFull){
		printf("Second Half:\n");
		for(int i=0;i<PROCESSED_BUF;i++){
			printf("%i ",ProcessedBuff2[i]);
		}
		secondHalfFull = false;


		printf("Cycles Taken:");
		for(int i=0; i< cyclesDebugLength;i++){
			if(cyclesDebug[i] == 0){

				break;
			}
			printf("%d ", cyclesDebug[i]);
		}


	}*/


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_DFSDM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_filter0.Instance = DFSDM1_Filter0;
  hdfsdm1_filter0.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter0.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter0.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter0.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC3_ORDER;
  hdfsdm1_filter0.Init.FilterParam.Oversampling = 50;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter0) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel2.Instance = DFSDM1_Channel2;
  hdfsdm1_channel2.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel2.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel2.Init.OutputClock.Divider = 100;
  hdfsdm1_channel2.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel2.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel2.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel2.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel2.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel2.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel2.Init.Awd.Oversampling = 1;
  hdfsdm1_channel2.Init.Offset = 0;
  hdfsdm1_channel2.Init.RightBitShift = 0;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  husart1.Instance = USART1;
  husart1.Init.BaudRate = 115200;
  husart1.Init.WordLength = USART_WORDLENGTH_8B;
  husart1.Init.StopBits = USART_STOPBITS_1;
  husart1.Init.Parity = USART_PARITY_NONE;
  husart1.Init.Mode = USART_MODE_TX_RX;
  husart1.Init.CLKPolarity = USART_POLARITY_LOW;
  husart1.Init.CLKPhase = USART_PHASE_1EDGE;
  husart1.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
