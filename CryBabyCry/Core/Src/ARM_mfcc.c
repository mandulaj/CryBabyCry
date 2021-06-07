

#include "ARM_mfcc.h"
#include "arm_math.h"
#include "CycleCounter.h"
#include <string.h>


#include <math.h>
#include <stdlib.h>

struct ARM_MFCC mfcc_inst;



void ARM_MFCC_Init(struct ARM_MFCC * mfcc){
	for (int i = 0; i < frame_len; i++)
		mfcc->window_func[i] = 0.5 - 0.5*cosf(M_2PI * ((float)i) / (frame_len));

	mfcc->mel_fbank = ARM_MFCC_create_mel_fbank(mfcc);
	mfcc->dct_matrix = ARM_MFCC_create_dct_matrix(NUM_FBANK_BINS, num_mfcc_features);
	arm_rfft_fast_init_f32(&mfcc->rfft, frame_len_padded);
}



float * ARM_MFCC_create_dct_matrix(int32_t input_length, int32_t coefficient_count) {
  int32_t k, n;
  float * M = malloc(sizeof(float) * input_length*coefficient_count);
  float normalizer;
  arm_sqrt_f32(2.0/(float)input_length,&normalizer);
  for (k = 0; k < coefficient_count; k++) {
    for (n = 0; n < input_length; n++) {
      M[k*input_length+n] = normalizer * cosf( ((double)M_PI)/input_length * (n + 0.5) * k );
    }
  }
  return M;
}

float ** ARM_MFCC_create_mel_fbank(struct ARM_MFCC * mfcc) {

  int32_t bin, i;

  int32_t num_fft_bins = frame_len_padded/2;
  float fft_bin_width = ((float)SAMP_FREQ) / frame_len_padded;
  float mel_low_freq = MelScale(MEL_LOW_FREQ);
  float mel_high_freq = MelScale(MEL_HIGH_FREQ);
  float mel_freq_delta = (mel_high_freq - mel_low_freq) / (NUM_FBANK_BINS+1);

  float *this_bin = malloc(sizeof(float) * num_fft_bins);

  float ** mel_fbank =  malloc(sizeof(float*) * NUM_FBANK_BINS);

  for (bin = 0; bin < NUM_FBANK_BINS; bin++) {

    float left_mel = mel_low_freq + bin * mel_freq_delta;
    float center_mel = mel_low_freq + (bin + 1) * mel_freq_delta;
    float right_mel = mel_low_freq + (bin + 2) * mel_freq_delta;

    int32_t first_index = -1, last_index = -1;

    for (i = 0; i < num_fft_bins; i++) {

      float freq = (fft_bin_width * i);  // center freq of this fft bin.
      float mel = MelScale(freq);
      this_bin[i] = 0.0;

      if (mel > left_mel && mel < right_mel) {
        float weight;
        if (mel <= center_mel) {
          weight = (mel - left_mel) / (center_mel - left_mel);
        } else {
          weight = (right_mel-mel) / (right_mel-center_mel);
        }
        this_bin[i] = weight;
        if (first_index == -1)
          first_index = i;
        last_index = i;
      }
    }

    mfcc->fbank_filter_first[bin] = first_index;
    mfcc->fbank_filter_last[bin] = last_index;
    mel_fbank[bin] = malloc(sizeof(float) * (last_index-first_index+1));

    int32_t j = 0;
    //copy the part we care about
    for (i = first_index; i <= last_index; i++) {
      mel_fbank[bin][j++] = this_bin[i];
    }
  }
  free(this_bin);
  return mel_fbank;
}


void ARM_MFCC_compute(struct ARM_MFCC * mfcc, const int16_t * audio_data, q7_t* mfcc_out) {

  int32_t i, j, bin;

  TS_START();
  TS_STAMP(0);

  //TensorFlow way of normalizing .wav data to (-1,1)
  for (i = 0; i < frame_len; i++) {
	  mfcc->frame[i] = (float)audio_data[i]/(1<<15);
  }
  //Fill up remaining with zeros
  memset(&mfcc->frame[frame_len], 0, sizeof(float) * (frame_len_padded-frame_len));
  TS_STAMP(1);


  for (i = 0; i < frame_len; i++) {
	  mfcc->frame[i] *= mfcc->window_func[i];
  }
  TS_STAMP(2);


  //Compute FFT
  arm_rfft_fast_f32(&mfcc->rfft, mfcc->frame, mfcc->buffer, 0);

  TS_STAMP(3);

  //Convert to power spectrum
  //frame is stored as [real0, realN/2-1, real1, im1, real2, im2, ...]
  int32_t half_dim = frame_len_padded/2;
  float first_energy = mfcc->buffer[0] * mfcc->buffer[0],
        last_energy =  mfcc->buffer[1] * mfcc->buffer[1];  // handle this special case
  for (i = 1; i < half_dim; i++) {
    float real = mfcc->buffer[i*2], im = mfcc->buffer[i*2 + 1];
    mfcc->buffer[i] = real*real + im*im;
  }
  mfcc->buffer[0] = first_energy;
  mfcc->buffer[half_dim] = last_energy;

  TS_STAMP(4);


  float sqrt_data;
  //Apply mel filterbanks
  for (bin = 0; bin < NUM_FBANK_BINS; bin++) {
    j = 0;
    float mel_energy = 0;
    int32_t first_index = mfcc->fbank_filter_first[bin];
    int32_t last_index = mfcc->fbank_filter_last[bin];
    for (i = first_index; i <= last_index; i++) {
      arm_sqrt_f32(mfcc->buffer[i],&sqrt_data);
      mel_energy += (sqrt_data) * mfcc->mel_fbank[bin][j];
      j++;
    }
    mfcc->mel_energies[bin] = mel_energy;

    //avoid log of zero
    if (mel_energy == 0.0)
    	mfcc->mel_energies[bin] = FLT_MIN;
  }

  TS_STAMP(5);


  //Take log
  for (bin = 0; bin < NUM_FBANK_BINS; bin++)
	  mfcc->mel_energies[bin] = 10.0f * log10f(fmaxf(1e-9,mfcc->mel_energies[bin]));

  TS_STAMP(6);

  //Take DCT. Uses matrix mul.
  for (i = 0; i < num_mfcc_features; i++) {
    float sum = 0.0;
    for (j = 0; j < NUM_FBANK_BINS; j++) {
      sum += mfcc->dct_matrix[i*NUM_FBANK_BINS+j] * mfcc->mel_energies[j];
    }

    //Input is Qx.mfcc_dec_bits (from quantization step)
    sum *= (0x1<<mfcc->mfcc_dec_bits);
    sum = round(sum);
    if(sum >= 127)
      mfcc_out[i] = 127;
    else if(sum <= -128)
      mfcc_out[i] = -128;
    else
      mfcc_out[i] = sum;
  }

  TS_STAMP(7);
  TS_STOP();


}
