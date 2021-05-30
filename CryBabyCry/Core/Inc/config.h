


#ifndef __CONFIG_H
#define __CONFIG_H

#define N_CEPS 40     // Number of ceps provided to the NN
#define N_FILTS 64    // Should be power of 2 for simpler FFT

#define CEPS_OFFSET 12		// Number of lower ceps discarded

#define MFCC_LENGTH 64     // Number of Frames in the MFCC


#define FRAME_LENGTH 512   // Number of sample in each MFCC frame ( should be power of 2)
#define REC_BUF_LENGTH (2*FRAME_LENGTH) // Buffer length for two consecutive frames

#define MODE_VALIDATION 0
#define MODE_DEMO 1


#define CURRENT_MODE MODE_DEMO

#endif
