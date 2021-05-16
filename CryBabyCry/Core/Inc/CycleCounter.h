#include "stm32l4xx_hal.h"


#ifndef CYCLECOUNTER
#define CYCLECOUNTER



// Resets the internal cycle counter to zero
void ResetTimer(void);

// Starts the internal cycle counter
void StartTimer(void);

// Stops the internal cycle counter
void StopTimer(void);

// Returns the current number of cycles according to the internal cycle counter
unsigned int getCycles(void);

#endif 
