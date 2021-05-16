


#ifndef _MFCC_H
#define _MFCC_H

#include "arm_math.h"


void MFCC_Init();
void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out);




#endif
