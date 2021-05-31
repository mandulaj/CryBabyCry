#include "stm32l4xx_hal.h"
#include "config.h"

#ifndef CYCLECOUNTER
#define CYCLECOUNTER



void ResetTimer(void);

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


inline uint32_t getCycles(){
	return *DWT_CYCCNT;
}




uint32_t TIME_STAMPS[N_TIME_STAMPS];


#ifdef ENABLE_TIME_STAMPS

#define TS_START() ResetTimer(); StartTimer()
#define TS_STAMP(a) TIME_STAMPS[a] = getCycles()
#define TS_STOP() StopTimer()

#else

#define TS_START() (void)0
#define TS_STAMP(a) (void)0
#define TS_STOP() (void)0


#endif






#endif 
