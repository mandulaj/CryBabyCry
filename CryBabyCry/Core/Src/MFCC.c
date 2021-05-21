

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "MFCC.h"
#include "arm_math.h"

#include "config.h"
#include "filter_coefs.h"

#include "math.h"



//static q31_t q31_iir_state[4];
static float32_t float_iir_state[4];

static float32_t Process_work_buffer[FRAME_LENGTH];
static float32_t Process_fft_output[FRAME_LENGTH];


static arm_biquad_casd_df1_inst_f32 iir_inst;
static arm_rfft_fast_instance_f32  f32_fft_inst;


static const uint32_t MFCC_bins[] = {
				  0,   0,   1,   2,   3,   4,   5,   6,   8,   9,  10,  11,  13,
				 14,  16,  17,  19,  20,  22,  24,  26,  28,  30,  32,  34,  36,
				 39,  41,  44,  46,  49,  52,  55,  58,  61,  64,  68,  71,  75,
				 79,  83,  87,  91,  96, 101, 106, 111, 116, 121, 127, 133, 139,
				146, 152, 159, 166, 174, 182, 190, 198, 207, 216, 225, 235, 245,
				256};









void MFCC_Init(){
	arm_biquad_cascade_df1_init_f32(&iir_inst, 1, float_biquad, float_iir_state);
	arm_rfft_fast_init_f32(&f32_fft_inst, FRAME_LENGTH);
}




#ifdef MFCC_FLOAT
float32_t dct_pState[N_FILTS*2];

void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out){
	float32_t mfs_output[N_FILTS];

#elif defined(MFCC_Q15)
float32_t MFCC_Process_Frame_temp_mfcc_buffer[N_FIasdfLTS];
void MFCC_Process_Frame(q15_t *inputBuf, q15_t *mfcc_out){
	float32_t *output = MFCC_Process_Frame_temp_mfcc_buffer;

#endif

	  // Copy raw buffer into a float work buffer
	  arm_q15_to_float(inputBuf, Process_work_buffer, FRAME_LENGTH);


// Cycles: 9054
	  // DC removing filter
	  arm_biquad_cascade_df1_f32(&iir_inst, Process_work_buffer, Process_work_buffer, FRAME_LENGTH);


// Cycles: 11303
//		  arm_biquad_casd_df1_inst_q31 iir_inst2;
//		  arm_biquad_cascade_df1_init_q31(&iir_inst2, 1, q31_biquad, q31_iir_state, 1);
//		  arm_biquad_cascade_df1_q31(&iir_inst2, buffer, bufferout, FFT_LEN);



	  // Multiply by hamming window
	  arm_mult_f32(Process_work_buffer, float_hamming_512, Process_work_buffer, FRAME_LENGTH);

	  // FFT input(Process_work_buffer) gets modified
	  arm_rfft_fast_f32(&f32_fft_inst, Process_work_buffer, Process_fft_output, 0);

	  // Compute magnitude
	  // result is half of the FRAME_LENGTH (half of spectrum)
	  arm_cmplx_mag_f32(Process_fft_output, Process_fft_output, FRAME_LENGTH/2);




	  memset(mfs_output, 0, N_FILTS*sizeof(*mfs_output));

	  /* For each filter */
	  for(uint32_t m = 1; m < N_FILTS+1; m++){
		  uint32_t f_m_minus = MFCC_bins[m-1];
		  uint32_t f_m = MFCC_bins[m];
		  uint32_t f_m_plus = MFCC_bins[m+1];

		  // Lower half of weighted bin
		  for(uint32_t k = f_m_minus; k < f_m; k++){
			  mfs_output[m-1] += Process_fft_output[k] *
					  (float32_t)(k - MFCC_bins[m - 1])/(float32_t)(MFCC_bins[m] - MFCC_bins[m - 1]);
		  }

		  // Upper half of weighted bin
		  for(uint32_t k = f_m; k < f_m_plus; k++){
			  mfs_output[m-1] += Process_fft_output[k] *
					  (float32_t)(MFCC_bins[m + 1] - k)/(float32_t)(MFCC_bins[m + 1] - MFCC_bins[m]);
		  }

	  }




//	  float32_t mean;
//	  arm_mean_f32(output, N_FILTS, &mean);
	  for (int i = 0; i < N_FILTS; i++){
		  mfs_output[i] = 10.0 * log10f(fmaxf(1e-9,mfs_output[i]));
	  }

	  dct2_64_f32(dct_pState, mfs_output, mfcc_out);

	  // Scale by factor of 1/sqrt(2*N_FILTS)
	  arm_scale_f32(mfcc_out, 0.08838834764831845 , mfcc_out, N_CEPS);


#ifdef MFCC_Q15
	  // Convert the float into q15
	  arm_float_to_q15(output, mfcc_out, N_FILTS);
#endif



}











#define dtc2_N 64


void dct2_64_f32(float32_t * pState, float32_t * pIn, float32_t * pOut){
	float32_t *pS1, *pS2, *pbuff;
	int32_t i;

	arm_rfft_fast_instance_f32 inst_rfft;

	arm_rfft_fast_init_f32(&inst_rfft, dtc2_N);




	/* pS1 initialized to pState */
	pS1 = pState;

	/* pS2 initialized to pState+N-1, so that it points to the end of the state buffer */
	pS2 = pState + (dtc2_N - 1U);

	/* pbuff initialized to input buffer */
	pbuff = pIn;



	/* Initializing the loop counter to N/2 >> 2 for loop unrolling by 4 */
	i = dtc2_N/2 >> 2U;

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
	i = dtc2_N >> 2U;

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


	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */

	float32_t *half1 = pState + 2;
	float32_t *half2 = pState + dtc2_N*2-1;
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
	arm_cmplx_mult_cmplx_f32 (pState, dct_weights_64, pState, dtc2_N);


	// Only output 60 ceps
	i = N_CEPS;
	pbuff = pOut;
	pS1 = pState+1; // Exclude the first bin
	do {
		*pbuff++ = 2 * *pS1++;
		pS1++;
		i--;
	} while(i > 0);




}





