/*
 * libmfcc.c - Implementation of the standard MFCC algorithm in C using FFTW library for computing FFT
 * Author - Akarsh Prabhakara
 *
 * MIT License
 * Copyright (c) 2017 Akarsh Prabhakara
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "arm_math.h"
#include "libmfcc.h"
#include "CycleCounter.h"

void mfcc(float *mfcc_result, float *data, int samplingRate, int nfilt, int numcep,
				int nfft, int ceplifter, int appendEnergy, int window, int numentries)

	// mfcc_result 		- stores the output of the MFCC computation - array of length numcep
	// data 			- input data of length numentries
	// samplingRate 	- the rate at which audio was sampled in Hz
	// nfilt 			- number of filters desired between 0 and samplingRate/2
	// numcep 			- number of MFCC coefficients
	// nfft 			- N in N-point FFT
	// ceplifter 		- 0 indicates no lifter
	// window 			- 0/1/2, 0 - rectangular, 1 - Hanning, 2 - Hamming

{
	TS_START();
	TS_STAMP(0);
	// Get frame data into temp_in and pad with zeros if numentries < nfft
	float *temp_in = (float *)malloc(nfft * sizeof(float));
	TS_STAMP(9);
	memset(temp_in, 0, nfft * sizeof(float));
	TS_STAMP(10);
	memcpy(temp_in, data, sizeof(float) * numentries);
	TS_STAMP(11);
	// Windowing
	if (window != 0)
		windowing(temp_in, numentries, window);
	TS_STAMP(1);
	// Run FFT
//	fftw_plan p;
//	fftw_complex *temp_out = (fftw_complex *)malloc((nfft / 2 + 1) * sizeof(float));
	float *temp_out = malloc((nfft/2+1) * sizeof(float) * 2);
//	p = fftw_plan_dft_r2c_1d(nfft, temp_in, temp_out, FFTW_ESTIMATE);
//	fftw_execute(p);
//	fftw_destroy_plan(p);

	arm_rfft_fast_instance_f32 rfft;
	arm_rfft_fast_init_f32(&rfft, nfft);
	arm_rfft_fast_f32(&rfft, temp_in, temp_out, 0);


	free(temp_in);
	TS_STAMP(2);
	// Compute power spectra
	float *powspectrum = (float *)malloc((nfft / 2 + 1) * sizeof(float));
	for (int i = 0; i < nfft / 2 + 1; i++)
		powspectrum[i] = (1.0 / nfft) * (powf(temp_out[2*i], 2) + powf(temp_out[2 * i + 1], 2));

	free(temp_out);
	TS_STAMP(3);

	// Compute the filterbank parameters
	float **fbank = (float **)malloc(nfilt * sizeof(float*));
	for (int i = 0; i < nfilt; i++)
		fbank[i] = (float *)malloc((nfft / 2 + 1) * sizeof(float));
	get_filterbank_parameters(fbank, nfilt, samplingRate, nfft);

	TS_STAMP(4);
	float specenergy = 0.0;
	for (int i = 0; i < nfft / 2 + 1; i++)
		specenergy += powspectrum[i];

	if (specenergy <= 0.0)
		specenergy = DBL_EPSILON;

	TS_STAMP(5);

	// Get filter bank output
	float *feat = (float *)malloc(nfilt * sizeof(float));
	for (int l = 0; l < nfilt; l++)
	{
		feat[l] = 0.0;
		for (int k = 0; k < nfft / 2 + 1; k++)
			feat[l] += powspectrum[k] * fbank[l][k];

		if (feat[l] > 0.0)
			feat[l] = logf(feat[l]);
		else
			feat[l] = DBL_EPSILON;
	}

	free(powspectrum);

	TS_STAMP(6);

	for (int i = 0; i < nfilt; i++)
		free(fbank[i]);
	free(fbank);

	TS_STAMP(7);

	for (int i = 0; i < numcep; i++)
	{
		// DCT - II of filter bank output
		mfcc_result[i] = 0.0;
		for (int j = 0; j < nfilt; j++)
			mfcc_result[i] += feat[j] * cosf((i * PI / nfilt) * (j + 0.5f));

		// Orthogonalization of DCT output
		if (i == 0)
			mfcc_result[i] *= sqrtf(1.0 / nfilt);
		else
			mfcc_result[i] *= sqrtf(2.0 / nfilt);

		// Ceplifter
		if (ceplifter != 0)
			mfcc_result[i] *= 1.0 + (ceplifter / 2.0) * sinf(PI * i / ceplifter);
	}

	free(feat);

	TS_STAMP(8);

	// Append Energy
	if (appendEnergy == 1)
		mfcc_result[0] = logf(specenergy);

	TS_STAMP(9);
	TS_STOP();
}

void get_filterbank_parameters(float **fbank, int nfilt, int samplingRate, int nfft)
{
	float lowmel = hztomel(0.0);
	float highmel = hztomel(samplingRate / 2.0);

	// Generate nfilt center frequencies linearly spaced in the mel scale
	float *bin = (float *)malloc((nfilt + 2) * sizeof(float));
	for (int i = 0; i <= nfilt + 1; i++)
		bin[i] = floorf(meltohz(i * (highmel - lowmel) / (nfilt + 1) + lowmel) * (nfft + 1) / samplingRate);

	// Triangular Filter Banks
	for (int i = 0; i < nfilt; i++)
	{
		memset(fbank[i], 0, (nfft / 2 + 1)*sizeof(float));
		for (int j = (int)bin[i]; j < (int)bin[i + 1]; j++)
			fbank[i][j] = (j - bin[i]) / (bin[i + 1] - bin[i]);
		for (int j = (int)bin[i + 1]; j < (int)bin[i + 2]; j++)
			fbank[i][j] = (bin[i + 2] - j) / (bin[i + 2] - bin[i + 1]);
	}
	free(bin);
}

float hztomel(float hz)
{
	return 2595 * log10f(1 + hz / 700.0);
}

float meltohz(float mel)
{
	return 700 * (powf(10, mel / 2595.0) - 1);
}

void windowing(float *temp_in, int numentries, int window)
{
	// Apply respective window before FFT
	for (int i = 0; i < numentries; i++)
	{
		if (window == 1)
			temp_in[i] *= (0.5 - 0.5 * cosf(2 * PI * i / (numentries - 1)));
		else if (window == 2)
			temp_in[i] *= (0.54 - 0.46 * cosf(2 * PI * i / (numentries - 1)));
	}
}
