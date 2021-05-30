#include "stm32l4xx_hal.h"


#ifndef CYCLECOUNTER
#define CYCLECOUNTER



void ResetTimer(void);

uint32_t getCycles(void);

void InitTimer();







volatile unsigned int *DWT_CYCCNT  ;
volatile unsigned int *DWT_CONTROL ;
volatile unsigned int *SCB_DEMCR   ;

// Resets the internal cycle counter to zero



// Starts the internal cycle counter

inline void StartTimer(){
	*DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter
}

// Stops the internal cycle counter

inline void StopTimer(){
	*DWT_CONTROL = *DWT_CONTROL & 0 ; // disable the counter
}






#endif 
