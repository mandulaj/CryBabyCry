


#ifndef _MFCC_H
#define _MFCC_H

#include "arm_math.h"
#include "config.h"

void MFCC_Init();

#ifdef MFCC_FLOAT

void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out);

#elif defined(MFCC_Q15)

void MFCC_Process_Frame(q15_t *inputBuf, q15_t *mfcc_out);

#endif


#endif
