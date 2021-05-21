


#ifndef _MFCC_H
#define _MFCC_H

#include "arm_math.h"
#include "config.h"

void MFCC_Init();

#ifdef MFCC_FLOAT

void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out);



void dct2_64_f32(float32_t * pState, float32_t * pIn, float32_t * pOut);

#elif defined(MFCC_Q15)

void MFCC_Process_Frame(q15_t *inputBuf, q15_t *mfcc_out);

#endif


#endif
