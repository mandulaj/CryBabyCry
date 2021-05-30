


#ifndef __AI_H
#define __AI_H

#include "arm_math.h"
#include "ai_datatypes_defines.h"



void AI_NN_init();
void AI_NN_inference(ai_i8 *input_data, ai_i8 *output_data);


void AI_quantize(float32_t * float_buffer, ai_i8 * quant_buffer, uint32_t length);
void AI_get_buffers(ai_i8 **input_data, ai_i8 **output_data);




#endif
