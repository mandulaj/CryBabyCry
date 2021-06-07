
#include "dct.h"

#include "arm_math.h"


arm_status dct2_init_f32(dct2_instance_f32 * S, arm_rfft_fast_instance_f32 * S_RFFT, uint16_t N){
	arm_status status = ARM_MATH_SUCCESS;


	S->N = N;
	S->Nby2 = N >> 1U;

	S->pRfft = S_RFFT;

	arm_rfft_fast_init_f32(S->pRfft, S->N);


	return status;
}

arm_status dct2_f32(dct2_instance_f32 * S, float32_t * pState, float32_t * pInlineBuffer){
	float32_t *pS1, *pS2, *pbuff;
	int32_t i;




	/* pS1 initialized to pState */
	pS1 = pState;

	/* pS2 initialized to pState+N-1, so that it points to the end of the state buffer */
	pS2 = pState + (S->N - 1U);

	/* pbuff initialized to input buffer */
	pbuff = pInlineBuffer;



	/* Initializing the loop counter to N/2 >> 2 for loop unrolling by 4 */
	i = S->Nby2 >> 2U;

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
	pbuff = pInlineBuffer;

	/* pS1 initialized to pState */
	pS1 = pState;

	/* Initializing the loop counter to N/4 instead of N for loop unrolling */
	i = S->N >> 2U;

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
	arm_rfft_fast_f32 (S->pRfft, pInlineBuffer, pState, 0);

	pState[N_FILTS] = pState[1];
	pState[1] = 0; 			// Set DC coef to 0
	pState[N_FILTS+1] = 0;

	/* Processing with loop unrolling 4 times as N is always multiple of 4.
	* Compute 4 outputs at a time */

	float32_t *half1 = pState + 2;
	float32_t *half2 = pState + 2 * S->N-1U;
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
	arm_cmplx_mult_cmplx_f32 (pState, S->pCosFactor, pState, S->N);

	// Only output N_CEPS ceps
	i = S->N;
	pbuff = pInlineBuffer;
	pS1 = pState; // Exclude the first CEPS_OFFSET bins
	do {
		*pbuff++ = *pS1;
		pS1+=2;
		i--;
	} while(i > 0);

}
