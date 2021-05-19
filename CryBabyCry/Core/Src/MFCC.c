

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


static const uint32_t MFCC_bins[] = {
			  	 0,   0,   1,   2,   4,   5,   6,   7,   8,  10,  11,  12,  14,
		        15,  17,  19,  21,  22,  24,  26,  28,  30,  33,  35,  38,  40,
		        43,  46,  48,  51,  55,  58,  61,  65,  68,  72,  76,  81,  85,
		        89,  94,  99, 104, 110, 115, 121, 127, 133, 140, 147, 154, 162,
		       169, 177, 186, 195, 204, 214, 223, 234, 245, 256};



void MFCC_Init(){
	arm_biquad_cascade_df1_init_f32(&iir_inst, 1, float_biquad, float_iir_state);
	arm_rfft_fast_init_f32(&f32_fft_inst, FRAME_LENGTH);
}




#ifdef MFCC_FLOAT
void MFCC_Process_Frame(q15_t *inputBuf, float32_t *mfcc_out){
	float32_t *output = mfcc_out;

#elif defined(MFCC_Q15)
float32_t MFCC_Process_Frame_temp_mfcc_buffer[N_FILTS];
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




	  memset(output, 0, N_FILTS*sizeof(*output));

	  /* For each filter */
	  for(uint32_t m = 1; m < N_FILTS+1; m++){
		  uint32_t f_m_minus = MFCC_bins[m-1];
		  uint32_t f_m = MFCC_bins[m];
		  uint32_t f_m_plus = MFCC_bins[m+1];

		  // Lower half of weighted bin
		  for(uint32_t k = f_m_minus; k < f_m; k++){
			  output[m-1] += Process_fft_output[k] *
					  (float32_t)(k - MFCC_bins[m - 1])/(float32_t)(MFCC_bins[m] - MFCC_bins[m - 1]);
		  }

		  // Upper half of weighted bin
		  for(uint32_t k = f_m; k < f_m_plus; k++){
			  output[m-1] += Process_fft_output[k] *
					  (float32_t)(MFCC_bins[m + 1] - k)/(float32_t)(MFCC_bins[m + 1] - MFCC_bins[m]);
		  }

	  }

//	  float32_t mean;
//	  arm_mean_f32(output, N_FILTS, &mean);



#ifdef MFCC_Q15
	  // Convert the float into q15
	  arm_float_to_q15(output, mfcc_out, N_FILTS);
#endif



}
