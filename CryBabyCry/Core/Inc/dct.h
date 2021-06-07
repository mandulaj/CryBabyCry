


#ifndef __DCT_H
#define __DCT_H
#include "arm_math.h"


typedef struct {
	uint16_t N;
	uint16_t Nby2;
const float32_t *pCosFactor;
	arm_rfft_fast_instance_f32 *pRfft;


} dct2_instance_f32;


arm_status dct2_init_f32(dct2_instance_f32 * S, arm_rfft_fast_instance_f32 * S_RFFT, uint16_t N);

arm_status dct2_f32(dct2_instance_f32 * S, float32_t * pState, float32_t * pInlineBuffer);









#endif
