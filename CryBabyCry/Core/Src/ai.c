/*
 * CryBabyCry - Code implementation
 * Copyright (c) 2021 Jakub Mandula
 *
 * This code is released under the MIT License.
 * For conditions of distribution and use, see the license in LICENSE
 */

#include <stdio.h>
#include "ai.h"

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "mfs_model.h"
#include "mfs_model_data.h"

#include "MFCC.h"
#include "cmsis_os.h"



// Buffer for storing activations
static AI_ALIGNED(4) ai_u8 model_activations[AI_MFS_MODEL_DATA_ACTIVATIONS_SIZE] ;

// Buffers for input and output data 
static AI_ALIGNED(4) ai_i8 in_model_data[AI_MFS_MODEL_IN_1_SIZE_BYTES] __attribute__((section(".ram2_bss")));
static AI_ALIGNED(4) ai_i8 out_model_data[AI_MFS_MODEL_OUT_1_SIZE_BYTES] __attribute__((section(".ram2_bss")));

// Model handle
static ai_handle mfs_model = AI_HANDLE_NULL;

// Input output model references
static ai_buffer ai_input[AI_MFS_MODEL_IN_NUM] = AI_MFS_MODEL_IN;
static ai_buffer ai_output[AI_MFS_MODEL_OUT_NUM] = AI_MFS_MODEL_OUT;




extern const ai_intq_info_list input_output_intq;

// Quantization scale and offset
static const float32_t AI_INPUT_ZERO_POINT = 3.0f;
static const float32_t AI_INPUT_SCALE = 0.05986613780260086f;




void AI_NN_init(){
	ai_error ai_err;


	ai_network_params ai_params = {
				AI_MFS_MODEL_DATA_WEIGHTS(ai_mfs_model_data_weights_get()),
				AI_MFS_MODEL_DATA_ACTIVATIONS(model_activations)
	};

	ai_err = ai_mfs_model_create(&mfs_model, AI_MFS_MODEL_DATA_CONFIG);
	if(ai_err.type != AI_ERROR_NONE){
		printf("Error, could not create NN instance\n");
		vTaskSuspend(NULL);
	}


	if(!ai_mfs_model_init(mfs_model, &ai_params)){
		printf("Error, could not init NN\n");
		vTaskSuspend(NULL);
	}
}


void AI_NN_inference(ai_i8 *input_data, ai_i8 *output_data){

	ai_i32 nbatch;

	// Set the input output pointers
	ai_input[0].n_batches = 1;
	ai_input[0].data = AI_HANDLE_PTR(input_data);
	ai_output[0].n_batches = 1;
	ai_output[0].data = AI_HANDLE_PTR(output_data);

	// Run the model
	nbatch  = ai_mfs_model_run(mfs_model, ai_input, ai_output);
	if (nbatch != 1) {
		printf("Error, could not run NN inference\n");
		vTaskSuspend(NULL);
	}

}




void AI_quantize(float32_t * float_buffer, ai_i8 * quant_buffer, uint32_t length){
	float32_t offset, scale;
	float32_t mean, std;

	// 151365 cycles
	//arm_offset_f32(pMFCC_Buff, -mean, pMFCC_Buff, MFCC_LENGTH*N_CEPS);
	//arm_scale_f32(pMFCC_Buff, 1.0f/std, pMFCC_Buff, MFCC_LENGTH*N_CEPS);

	// Subtract the mean and divide by std
	// combined with divide by scale and add zero point


	//offset = AI_INPUT_ZERO_POINT * AI_INPUT_SCALE * std - mean;
	//scale = AI_INPUT_SCALE * std * 128.0f; // Divide Factor of 128 because arm_float_to_q7 multiplies by 128

	// Calculate the mean and std of the MFCC samples
	arm_mean_f32(float_buffer, length, &mean);
	arm_std_f32(float_buffer, length, &std);

	// Result should have 0 mean and be offset by AI_ZERO_POINT
	// The std should be 1, prescaled by AI_INPUT_SCALE and prepared for q7 conversion
	offset = -mean + AI_INPUT_ZERO_POINT * std * AI_INPUT_SCALE;
	scale = 1.0f/(128.0f * std * AI_INPUT_SCALE);


	// Perform Normalization Scaling
	fast_offset_scale_f32(float_buffer, offset, scale, float_buffer, length);


	// Convert to uint8
	arm_float_to_q7(float_buffer, (q7_t *)quant_buffer, length);
}


void AI_get_buffers(ai_i8 **input_data, ai_i8 **output_data) {
	*input_data = in_model_data;
	*output_data = out_model_data;
}



