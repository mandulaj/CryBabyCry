


#ifndef __ARM_MFCC_H
#define __ARM_MFCC_H

#include "arm_math.h"

#define M_2PI 6.283185307179586476925286766559005
#define SAMP_FREQ 16000
#define NUM_FBANK_BINS 64
#define MEL_LOW_FREQ 20
#define MEL_HIGH_FREQ 4000

#define frame_len_padded 512
#define frame_len 512
#define num_mfcc_features 64

struct ARM_MFCC {
	float32_t frame[frame_len_padded];
	float32_t buffer[frame_len_padded];
	float32_t mel_energies[NUM_FBANK_BINS];

	float32_t window_func[frame_len];
	int32_t fbank_filter_first[NUM_FBANK_BINS];
	int32_t fbank_filter_last[NUM_FBANK_BINS];
	float32_t ** mel_fbank;
	float32_t * dct_matrix;
	arm_rfft_fast_instance_f32 rfft;

	int mfcc_dec_bits;

};


void ARM_MFCC_Init(struct ARM_MFCC * mfcc);

float * ARM_MFCC_create_dct_matrix(int32_t input_length, int32_t coefficient_count) ;
float ** ARM_MFCC_create_mel_fbank(struct ARM_MFCC * mfcc) ;
void ARM_MFCC_compute(struct ARM_MFCC * mfcc, const int16_t * audio_data, q7_t* mfcc_out);



static inline float InverseMelScale(float mel_freq) {
     return 700.0f * (expf (mel_freq / 1127.0f) - 1.0f);
}

static inline float MelScale(float freq) {
     return 1127.0f * logf (1.0f + freq / 700.0f);
}




#endif
