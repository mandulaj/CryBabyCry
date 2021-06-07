/*
 * libmfcc.h - Header file for libmfcc.c
 * Author - Akarsh Prabhakara
 *
 * MIT License
 * Copyright (c) 2017 Akarsh Prabhakara
 */

#pragma once

//#define DBL_EPSILON 2.2204460492503131e-16
//#define PI 3.14159265358979323846264338327

void mfcc(float *mfcc_result, float *data, int samplingRate, int nfilt, int numcep, int nfft, int ceplifter, int appendEnergy, int window, int numentries);
void get_filterbank_parameters(float **fbank, int nfilt, int samplingRate, int nfft);
float hztomel(float hz);
float meltohz(float mel);
void windowing(float *temp_in, int numentries, int window);
