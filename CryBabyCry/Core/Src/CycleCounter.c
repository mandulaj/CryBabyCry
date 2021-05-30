#include "CycleCounter.h"



//static uint32_t cycle_offset = 0;

void InitTimer(){
	DWT_CYCCNT   = (unsigned int *)0xE0001004; //address of the register
	DWT_CONTROL  = (unsigned int *)0xE0001000; //address of the register
	SCB_DEMCR    = (unsigned int *)0xE000EDFC; //address of the register
}


void ResetTimer(){
	*SCB_DEMCR   = *SCB_DEMCR | 0x01000000;
	*DWT_CONTROL = 0;
	*DWT_CYCCNT  = 0; // reset the counter

}


// Returns the current number of cycles according to the internal cycle counter
uint32_t getCycles(){
	return *DWT_CYCCNT;
}
