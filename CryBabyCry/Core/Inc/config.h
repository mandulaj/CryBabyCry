


#ifndef __CONFIG_H
#define __CONFIG_H


#define N_FILTS 60
#define MFCC_LENGTH 128


#define FRAME_LENGTH 512
#define REC_BUF_LENGTH (2*FRAME_LENGTH)


// MFCC Type
#define MFCC_FLOAT
#ifndef MFCC_FLOAT
#define MFCC_Q15
#endif


#endif
