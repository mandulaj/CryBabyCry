/*
 * CryBabyCry - Code implementation
 * Copyright (c) 2021 Jakub Mandula
 *
 * This code is released under the MIT License.
 * For conditions of distribution and use, see the license in LICENSE
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "MFCC.h"
#include "arm_math.h"

#include "config.h"
#include "filter_coefs.h"

#include "math.h"

#include "CycleCounter.h"


#ifdef FLOAT_MFCC
//static q31_t q31_iir_state[4];
static float32_t float_iir_state[4];

static float32_t Process_work_buffer[FRAME_LENGTH];
static float32_t Process_fft_output[FRAME_LENGTH+2]; // Account for rfft being N+1

static float32_t dct_pState[N_FILTS*2];



static arm_biquad_casd_df1_inst_f32 iir_inst;
static arm_rfft_fast_instance_f32  f32_fft_inst;
static arm_rfft_fast_instance_f32  inst_rfft;

#else
static q15_t q15_iir_state[4];

static q15_t Process_work_buffer[FRAME_LENGTH];
static q15_t Process_fft_output[FRAME_LENGTH+2]; // Account for rfft being N+1

static q15_t dct_pState[N_FILTS*2];



static arm_biquad_casd_df1_inst_q15 q15_iir_inst;
static arm_rfft_instance_q15  q15_fft_inst;
static arm_rfft_instance_q15  q15_inst_rfft;


#endif


void MFCC_Init(){
#ifdef FLOAT_MFCC
	arm_biquad_cascade_df1_init_f32(&iir_inst, 1, float_biquad, float_iir_state);
	arm_rfft_fast_init_f32(&f32_fft_inst, FRAME_LENGTH);
	arm_rfft_fast_init_f32(&inst_rfft, N_FILTS);
#else
	arm_biquad_cascade_df1_init_q15(&q15_iir_inst, 1, q15_biquad, q15_iir_state, 2);
	arm_rfft_init_q15(&q15_fft_inst, FRAME_LENGTH, 0, 0);
	arm_rfft_init_q15(&q15_inst_rfft, N_FILTS, 0, 0);
#endif
}



#define safe_10log10f(x) (4.342944819032518276511f * logf(fmaxf(1e-9f, x)))


#ifdef FLOAT_MFCC

void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out){
	float32_t mfs_output[N_FILTS];

	TS_START();
	TS_STAMP(0);

	// Copy raw buffer into a float work buffer
	arm_q15_to_float(inputBuf, Process_work_buffer, FRAME_LENGTH);

	TS_STAMP(1);


	// Cyclesb: 9054
	// DC removing filter
	arm_biquad_cascade_df1_f32(&iir_inst, Process_work_buffer, Process_work_buffer, FRAME_LENGTH);
	// Cycles: 11303
	//		  arm_biquad_casd_df1_inst_q31 iir_inst2;
	//		  arm_biquad_cascade_df1_init_q31(&iir_inst2, 1, q31_biquad, q31_iir_state, 1);
	//		  arm_biquad_cascade_df1_q31(&iir_inst2, buffer, bufferout, FFT_LEN);

	TS_STAMP(2);

	// Multiply by hamming window
	arm_mult_f32(Process_work_buffer, float_hamming_512, Process_work_buffer, FRAME_LENGTH);

	TS_STAMP(3);

	// FFT input(Process_work_buffer) gets modified
	arm_rfft_fast_f32(&f32_fft_inst, Process_work_buffer, Process_fft_output, 0);

	// The rfft function outputs the upper coeff in second position
	// Make sure the DC is real
	Process_fft_output[FRAME_LENGTH] = Process_fft_output[1];
	Process_fft_output[1] = 0; // Set complex component to 0
	Process_fft_output[FRAME_LENGTH+1] = 0; // Set complex component to 0

	TS_STAMP(4);

	// Compute magnitude
	// result is half of the FRAME_LENGTH (half of spectrum)
	arm_cmplx_mag_f32(Process_fft_output, Process_fft_output, (FRAME_LENGTH/2) + 1);

	TS_STAMP(5);

	memset(mfs_output, 0, N_FILTS*sizeof(*mfs_output));

	/* For each filter */
	const float32_t *pBins_factors = MFCC_bins_factors;
	// For each mel bin m
	for(uint32_t m = 0; m < N_FILTS; m++){
		uint32_t bin_pos = MFCC_bins_start[m]; // Bin position in FFT spectrum
		uint32_t count = MFCC_bins_length[m];  // Number of bins for current mel
		do {

			// Scale FFT bin with given bin factor
			mfs_output[m] += Process_fft_output[bin_pos++] * (*pBins_factors++);
			count--;
		} while (count > 0);


	}

	TS_STAMP(6);

	//	  float32_t mean;
	//	  arm_mean_f32(output, N_FILTS, &mean);
	for (int i = 0; i < N_FILTS; i++){
		// Take the 10.0 * log10(x) in a safe manner
		mfs_output[i] =  safe_10log10f(mfs_output[i]);
	}

	TS_STAMP(7);

	dct2_64_f32(dct_pState, mfs_output, mfcc_out);

	TS_STAMP(8);

	TS_STOP();
}






void dct2_64_f32(float32_t * pState, float32_t * pIn, float32_t * pOut){
	float32_t *pS1, *pS2, *pbuff;
	int32_t i;



	// Clear the pState buffer
	memset(pState, 0, N_FILTS * 2 * sizeof(*pState));


	/* pS1 initialized to pState */
	pS1 = pState;

	/* pS2 initialized to pState+N-1, so that it points to the end of the state buffer */
	pS2 = pState + (N_FILTS - 1U);

	/* pbuff initialized to input buffer */
	pbuff = pIn;



	/* Initializing the loop counter to N/2 >> 2 for loop unrolling by 4 */
	i = N_FILTS/2 >> 2U;

	/* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
	 ** a second loop below computes the remaining 1 to 3 samples. */
	do
	{
		/* Re-ordering of even and odd elements */
		/* pState[i] =  pInlineBuffer[2*i] */
		*pS1++ = *pbuff++;
		/* pState[N-i-1] = pInlineBuffer[2*i+1] */
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		/* Decrement loop counter */
		i--;
	} while (i > 0U);

	/* pbuff initialized to input buffer */
	pbuff = pIn;

	/* pS1 initialized to pState */
	pS1 = pState;

	/* Initializing the loop counter to N/4 instead of N for loop unrolling */
	i = N_FILTS >> 2U;

	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */
	do
	{
		/* Writing the re-ordered output back to inplace input buffer */
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;

		/* Decrement the loop counter */
		i--;
	} while (i > 0U);


	/* ---------------------------------------------------------
	*     Step2: Calculate RFFT for N-point input
	* ---------------------------------------------------------- */
	/* pInlineBuffer is real input of length N , pState is the complex output of length 2N */
	arm_rfft_fast_f32 (&inst_rfft, pIn, pState, 0);

	pState[N_FILTS] = pState[1];
	pState[1] = 0; 			// Set DC coef to 0
	pState[N_FILTS+1] = 0;

	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */

	float32_t *half1 = pState + 2;
	float32_t *half2 = pState + N_FILTS*2-1;
	do
	{
		/* Writing the re-ordered output back to inplace input buffer */
		float32_t real = *half1++;
		float32_t imag = *half1++;
		*half2-- = -imag;
		*half2-- = real;
	} while (half1 < half2);


	/*----------------------------------------------------------------------
	*  Step3: Multiply the FFT output with the weights.
	*----------------------------------------------------------------------*/
	// Multiply with 2 * np.exp(-1j*pi*k/(2*N)) * orth
	arm_cmplx_mult_cmplx_f32 (pState, dct_weights_64_orth, pState, N_FILTS);

	// Only output N_CEPS ceps
	i = N_CEPS;
	pbuff = pOut;
	pS1 = pState + CEPS_OFFSET * 2; // Exclude the first CEPS_OFFSET bins
	do {
		*pbuff++ = *pS1++;
		pS1++;
		i--;
	} while(i > 0);

}
#else
void MFCC_Process_Frame(q15_t *inputBuf, q15_t *mfcc_out){
	q15_t mfs_output[N_FILTS];


	TS_START();
	TS_STAMP(0);
	// Copy raw buffer into a float work buffer
//	arm_q15_to_float(inputBuf, Process_work_buffer, FRAME_LENGTH);

	TS_STAMP(1);

	// Cyclesb: 9054
	// DC removing filter
	arm_biquad_cascade_df1_fast_q15(&q15_iir_inst, inputBuf, Process_work_buffer, FRAME_LENGTH);
	// Cycles: 11303
	//		  arm_biquad_casd_df1_inst_q31 iir_inst2;
	//		  arm_biquad_cascade_df1_init_q31(&iir_inst2, 1, q31_biquad, q31_iir_state, 1);
	//		  arm_biquad_cascade_df1_q31(&iir_inst2, buffer, bufferout, FFT_LEN);

	TS_STAMP(2);
	// Multiply by hamming window
	arm_mult_q15(Process_work_buffer, q15_hamming_512, Process_work_buffer, FRAME_LENGTH);

	TS_STAMP(3);
	// FFT input(Process_work_buffer) gets modified
	arm_rfft_q15(&q15_fft_inst, Process_work_buffer, Process_fft_output);

	// The rfft function outputs the upper coeff in second position
	// Make sure the DC is real
	Process_fft_output[FRAME_LENGTH] = Process_fft_output[1];
	Process_fft_output[1] = 0; // Set complex component to 0
	Process_fft_output[FRAME_LENGTH+1] = 0; // Set complex component to 0

	TS_STAMP(4);
	// Compute magnitude
	// result is half of the FRAME_LENGTH (half of spectrum)
	arm_cmplx_mag_q15(Process_fft_output, Process_fft_output, (FRAME_LENGTH/2) + 1);

	TS_STAMP(5);

	memset(mfs_output, 0, N_FILTS*sizeof(*mfs_output));

	/* For each filter */
	const q15_t *pBins_factors = MFCC_bins_factors_q15;
	// For each mel bin m
	for(uint32_t m = 0; m < N_FILTS; m++){
		uint32_t bin_pos = MFCC_bins_start[m]; // Bin position in FFT spectrum
		uint32_t count = MFCC_bins_length[m];  // Number of bins for current mel
		do {

			// Scale FFT bin with given bin factor
			mfs_output[m] += Process_fft_output[bin_pos++] * (*pBins_factors++);
			count--;
		} while (count > 0);


	}
	TS_STAMP(6);
	//	  float32_t mean;
	//	  arm_mean_f32(output, N_FILTS, &mean);
	for (int i = 0; i < N_FILTS; i++){
		// Take the 10.0 * log10(x) in a safe manner
		mfs_output[i] =  safe_10log10f(mfs_output[i]);
	}
	TS_STAMP(7);

	dct2_64_q15(dct_pState, mfs_output, mfcc_out);
	TS_STAMP(8);
	TS_STOP();
}






void dct2_64_q15(q15_t * pState, q15_t * pIn, q15_t * pOut){
	q15_t *pS1, *pS2, *pbuff;
	int32_t i;



	// Clear the pState buffer
	memset(pState, 0, N_FILTS * 2 * sizeof(*pState));


	/* pS1 initialized to pState */
	pS1 = pState;

	/* pS2 initialized to pState+N-1, so that it points to the end of the state buffer */
	pS2 = pState + (N_FILTS - 1U);

	/* pbuff initialized to input buffer */
	pbuff = pIn;



	/* Initializing the loop counter to N/2 >> 2 for loop unrolling by 4 */
	i = N_FILTS/2 >> 2U;

	/* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
	 ** a second loop below computes the remaining 1 to 3 samples. */
	do
	{
		/* Re-ordering of even and odd elements */
		/* pState[i] =  pInlineBuffer[2*i] */
		*pS1++ = *pbuff++;
		/* pState[N-i-1] = pInlineBuffer[2*i+1] */
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		*pS1++ = *pbuff++;
		*pS2-- = *pbuff++;

		/* Decrement loop counter */
		i--;
	} while (i > 0U);

	/* pbuff initialized to input buffer */
	pbuff = pIn;

	/* pS1 initialized to pState */
	pS1 = pState;

	/* Initializing the loop counter to N/4 instead of N for loop unrolling */
	i = N_FILTS >> 2U;

	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */
	do
	{
		/* Writing the re-ordered output back to inplace input buffer */
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;
		*pbuff++ = *pS1++;

		/* Decrement the loop counter */
		i--;
	} while (i > 0U);


	/* ---------------------------------------------------------
	*     Step2: Calculate RFFT for N-point input
	* ---------------------------------------------------------- */
	/* pInlineBuffer is real input of length N , pState is the complex output of length 2N */
	arm_rfft_q15 (&q15_inst_rfft, pIn, pState);

	pState[N_FILTS] = pState[1];
	pState[1] = 0; 			// Set DC coef to 0
	pState[N_FILTS+1] = 0;

	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */

	q15_t *half1 = pState + 2;
	q15_t *half2 = pState + N_FILTS*2-1;
	do
	{
		/* Writing the re-ordered output back to inplace input buffer */
		q15_t real = *half1++;
		q15_t imag = *half1++;
		*half2-- = -imag;
		*half2-- = real;
	} while (half1 < half2);


	/*----------------------------------------------------------------------
	*  Step3: Multiply the FFT output with the weights.
	*----------------------------------------------------------------------*/
	// Multiply with 2 * np.exp(-1j*pi*k/(2*N)) * orth
	arm_cmplx_mult_cmplx_q15 (pState, q15_dct_weights_64, pState, N_FILTS);

	// Only output N_CEPS ceps
	i = N_CEPS;
	pbuff = pOut;
	pS1 = pState + CEPS_OFFSET * 2; // Exclude the first CEPS_OFFSET bins
	do {
		*pbuff++ = *pS1++;
		pS1++;
		i--;
	} while(i > 0);

}


#endif

void fast_offset_scale_f32(float32_t *pSrc, float32_t offset, float32_t scale, float32_t *pDst, uint32_t blockSize){
	uint32_t blkCnt;

	blkCnt = blockSize >> 2U;

	do {
		*pDst++ = ((*pSrc++) + offset) * scale;
		*pDst++ = ((*pSrc++) + offset) * scale;
		*pDst++ = ((*pSrc++) + offset) * scale;
		*pDst++ = ((*pSrc++) + offset) * scale;

		blkCnt--;
	} while (blkCnt > 0U);

	blkCnt = blockSize % 0x4U;


	while (blkCnt > 0U){
		*pDst++ = ((*pSrc++) + offset) * scale;
		blkCnt--;
	}
}








