#ifndef __HELPER_H
#define __HELPER_H

#include "arm_math.h"
#include "config.h"





struct CPB{
	// Contiguous memory for internal memory buffer
	q15_t _internalBuffer[FRAME_LENGTH + (FRAME_LENGTH/2)];
	q15_t *full_frame;
	q15_t *half_frame;

};

void CPB_Init(struct CPB *cpb);
void CPB_copyFull(struct CPB *cpb);


void print_buffer_f32(float32_t *buf, uint32_t len);
void print_buffer_q31(q31_t *buf, uint32_t len);
void print_buffer_q15(q15_t *buf, uint32_t len);

void print_buffer_grid_f32(float32_t * buf, uint32_t row, uint32_t columns);
void print_buffer_grid_i32(int32_t * buf, uint32_t row, uint32_t columns);
void print_buffer_grid_q7(q7_t * buf, uint32_t row, uint32_t columns);

//void USART1_printf(char *fmt, ...);




#endif
