/*
 * CryBabyCry - Code implementation
 * Copyright (c) 2021 Jakub Mandula
 *
 * This code is released under the MIT License.
 * For conditions of distribution and use, see the license in LICENSE
 */


#include <validation.h>
#include <stdio.h>
#include <ai.h>
#include "arm_math.h"
#include "config.h"
#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"
#include <stdbool.h>
#include <string.h>
#include "helper.h"
#include "CycleCounter.h"
#include "tim.h"
#include "MFCC.h"



extern USART_HandleTypeDef husart1;
extern UART_HandleTypeDef huart4;

extern float32_t MFCC_Buffer1[MFCC_LENGTH][N_CEPS];

#if(CURRENT_MODE==MODE_VALIDATION)



static struct CPB cpb_struct;

volatile bool done = false;


void Read_frame(){



//	for(uint32_t i = 0; i< FRAME_LENGTH; i++){
//		cpb_struct.full_frame[i] = (q15_t) Read_Next_Sample();
//	}

	print_buffer_grid_q15(cpb_struct.full_frame, FRAME_LENGTH/16, 16);
		//	printf("%s\n",HAL_UART_ReceiveUART_RcvBuff);
		//	printf("Sample: %ld\n", sample);

}



float32_t CycleTimes_MFCC[MFCC_LENGTH/2];
float32_t CycleTimes_COM[MFCC_LENGTH/2];

bool Process_validation_Example(){
	uint32_t MFCC_index = 0;
	ai_i8 *nn_input_buffer;
	ai_i8 *nn_output_buffer;
	int8_t cry,other;

	float32_t CycleTimes_Quantize, CycleTimes_inference;


	CPB_reset(&cpb_struct);
	AI_get_buffers(&nn_input_buffer, &nn_output_buffer);

	HAL_TIM_Base_Start(&htim16);
	htim16.Instance->CNT = 0;


	uint32_t timestamp = htim16.Instance->CNT;

	// Get label
	uint8_t label;
	HAL_UART_Transmit(&huart4, (uint8_t *)"\n", 1U, 0xFFFF);
	HAL_UART_Receive(&huart4, (uint8_t *)&label, 1, 0xFFFF);


	do{
		// Fill Frame with next sample
		// We can get next char
		ResetTimer();
		StartTimer();
		HAL_UART_Transmit(&huart4, (uint8_t *)"\n", 1U, 0xFFFF);
		HAL_UART_Receive_DMA(&huart4, (uint8_t *)cpb_struct.full_frame, FRAME_LENGTH*2);
		while(!done);
		done = false;
		StopTimer();
		CycleTimes_COM[MFCC_index/2] = getCycles();

		ResetTimer();
		StartTimer();
		MFCC_Process_Frame(cpb_struct.half_frame, MFCC_Buffer1[MFCC_index++]);
		MFCC_Process_Frame(cpb_struct.full_frame, MFCC_Buffer1[MFCC_index++]);

		// Swap buffers
		CPB_copyFull(&cpb_struct);
		StopTimer();
		CycleTimes_MFCC[MFCC_index/2 - 1] = getCycles();


	} while(MFCC_index < MFCC_LENGTH);

	ResetTimer();
	StartTimer();
	AI_quantize((float32_t *)MFCC_Buffer1, nn_input_buffer, MFCC_LENGTH*N_CEPS);
	StopTimer();
	CycleTimes_Quantize = getCycles();



	ResetTimer();
	StartTimer();
	AI_NN_inference(nn_input_buffer, nn_output_buffer);
	StopTimer();
	CycleTimes_inference = getCycles();

	cry = nn_output_buffer[0];
	other = nn_output_buffer[1];


	uint32_t timestamp2 = htim16.Instance->CNT;

	float32_t mean_mfcc;
	arm_mean_f32(CycleTimes_MFCC, MFCC_LENGTH/2, &mean_mfcc);
	mean_mfcc *= MFCC_LENGTH/2;

	float32_t mean_com;
	arm_mean_f32(CycleTimes_COM, MFCC_LENGTH/2, &mean_com);
	mean_com *= MFCC_LENGTH/2;

	printf("================\n");
	printf("Detected: %s True: %s\n",(cry>other)? "CRY" : "OTHER", (!label)? "CRY" : "OTHER");
	printf("================\n");
	printf("cry: %d other: %d\n", cry, other);
	printf("MFCC Time: %.2fms, COM Time: %.2fms, Quantization Time: %.2fus, Inference Time: %.2fms\n",
			mean_mfcc / 80000,
			mean_com/ 80000,
			CycleTimes_Quantize / 80,
			CycleTimes_inference / 80000
			);
	printf("Total: %fms\n", (timestamp2- timestamp)/100.0f);

	if((cry<other) == label){
		printf("Correct\n");
		return true;
	} else {
		printf("Incorrect\n");
		return false;

	}
}

#define NUM_VAL 100


void Validation_Start(){



	AI_NN_init();

	CPB_Init(&cpb_struct);
	uint32_t correct = 0;

	printf("Reading into buffer\n");
//	HAL_UART_Transmit(&huart4, "Hello World\n", 12, 0xFFFF);
	for(uint32_t i = 0; i < NUM_VAL; i++){
		if(Process_validation_Example())
			correct += 1;
	}


	printf("Done\n");
	printf("====================================\n");
	printf("====================================\n");
	printf("====================================\n");
	printf("Overall %ld/%ld correct = %.2f%%\n",correct, NUM_VAL, correct/(float32_t)NUM_VAL*100.0f);




//	AI_NN_inference();



}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *husart){

//	printf("Done reading %s\n", buffer);
	done = true;
}




#endif
