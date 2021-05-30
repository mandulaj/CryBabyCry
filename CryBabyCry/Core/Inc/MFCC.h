/*
 * CryBabyCry - Code implementation
 * Copyright (c) 2021 Jakub Mandula
 *
 * This code is released under the MIT License.
 * For conditions of distribution and use, see the license in LICENSE
 */


#ifndef _MFCC_H
#define _MFCC_H

#include "arm_math.h"
#include "config.h"

void MFCC_Init();



void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out);

void dct2_64_f32(float32_t * pState, float32_t * pIn, float32_t * pOut);

void fast_offset_scale_f32(float32_t *pSrc, float32_t offset, float32_t scale, float32_t *pDst, uint32_t blockSize);





#endif
