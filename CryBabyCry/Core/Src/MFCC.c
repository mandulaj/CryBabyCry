

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "MFCC.h"
#include "arm_math.h"

#include "config.h"
#include "filter_coefs.h"


//static q31_t q31_iir_state[4];
static float32_t float_iir_state[4];

static float32_t Process_work_buffer[FRAME_LENGTH];
static float32_t Process_fft_output[FRAME_LENGTH];


static arm_biquad_casd_df1_inst_f32 iir_inst;
static arm_rfft_fast_instance_f32  f32_fft_inst;

void MFCC_Init(){
	arm_biquad_cascade_df1_init_f32(&iir_inst, 1, float_biquad, float_iir_state);
	arm_rfft_fast_init_f32(&f32_fft_inst, FRAME_LENGTH);
}


void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out){


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

	  // FFT
	  arm_rfft_fast_f32(&f32_fft_inst, Process_work_buffer, Process_fft_output, 0);

	  // Compute magnitude
	  arm_cmplx_mag_squared_f32(Process_fft_output, Process_fft_output, FRAME_LENGTH/2);


	  const uint32_t bins[] = {
			  	 0,   0,   1,   2,   4,   5,   6,   7,   8,  10,  11,  12,  14,
		        15,  17,  19,  21,  22,  24,  26,  28,  30,  33,  35,  38,  40,
		        43,  46,  48,  51,  55,  58,  61,  65,  68,  72,  76,  81,  85,
		        89,  94,  99, 104, 110, 115, 121, 127, 133, 140, 147, 154, 162,
		       169, 177, 186, 195, 204, 214, 223, 234, 245, 256};


	  memset(mfcc_out, 0, N_FILTS*sizeof(float32_t));

	  /* For each filter */
	  for(uint32_t m = 1; m < N_FILTS+1; m++){
		  uint32_t f_m_minus = bins[m-1];
		  uint32_t f_m = bins[m];
		  uint32_t f_m_plus = bins[m+1];

		  for(uint32_t k = f_m_minus; k < f_m; k++){
			  mfcc_out[m-1] += Process_fft_output[k] * (float32_t)(k - bins[m - 1])/(float32_t)(bins[m] - bins[m - 1]);
		  }
		  for(uint32_t k = f_m; k < f_m_plus; k++){
			  mfcc_out[m-1] += Process_fft_output[k] * (float32_t)(bins[m + 1] - k)/(float32_t)(bins[m + 1] - bins[m]);
		  }

	  }


}
