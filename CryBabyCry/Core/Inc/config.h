/*
 * CryBabyCry - Code implementation
 * Copyright (c) 2021 Jakub Mandula
 *
 * This code is released under the MIT License.
 * For conditions of distribution and use, see the license in LICENSE
 */


#ifndef __CONFIG_H
#define __CONFIG_H

/* Configuration of the sample acquisition */
#define FRAME_LENGTH 512   					// Number of sample in each MFCC frame ( should be power of 2)
#define REC_BUF_LENGTH (2*FRAME_LENGTH) 	// Buffer length for two consecutive frames

/* Configuration of the MFCC calculation */
#define N_CEPS 40     						// Number of ceps provided to the NN
#define N_FILTS 64    						// Should be power of 2 for simpler FFT

#define CEPS_OFFSET 12						// Number of lower ceps discarded

#define MFCC_LENGTH 64     					// Number of Frames in the MFCC

#define FLOAT_MFCC         					// Compute using floats. Disable to use q15


#define NUM_VAL 1000                        // In validation mode, number of samples to read


// Mode definitions
#define MODE_VALIDATION 0
#define MODE_DEMO 1

/* Set the mode of the application */
#define CURRENT_MODE MODE_DEMO

/* Benchmarking Time stamps */
#define N_TIME_STAMPS 20                    // Length of time stamp buffer
#define ENABLE_TIME_STAMPS					// Enable timestamping

#endif
