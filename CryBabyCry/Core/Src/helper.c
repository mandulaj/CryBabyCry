

#include "helper.h"
#include <stdio.h>
#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"

extern USART_HandleTypeDef husart1;

void CPB_Init(struct CPB *cpb){
	cpb->full_frame = cpb->_internalBuffer + (FRAME_LENGTH/2);
	cpb->half_frame = cpb->_internalBuffer;
}


void CPB_copyFull(struct CPB *cpb){
	memcpy(cpb->half_frame, cpb->full_frame + (FRAME_LENGTH/2), FRAME_LENGTH/2 * sizeof(uint16_t));
}



void print_buffer_f32(float32_t *buf, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		printf("%f ", buf[i]);
	}

}

void print_buffer_q31(q31_t *buf, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		printf("%ld ", buf[i]);
	}

}

void print_buffer_q15(q15_t *buf, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		printf("%i ", buf[i]);
	}

}


//#define PRINT_LEN 1024
//static char USART_print_buf[PRINT_LEN];

//void USART1_printf(char *fmt, ...){
//	va_list arg_ptr;
//
//	va_start(arg_ptr, fmt);
//
//	uint32_t len = vsnprintf(USART_print_buf, PRINT_LEN, fmt, arg_ptr);
//
//	HAL_USART_Transmit(&husart1, (uint8_t *) USART_print_buf, len, HAL_MAX_DELAY);
//
//
//	va_end(arg_ptr);
//
//}


int _write(int fd, char * ptr, int len)
{
  HAL_USART_Transmit(&husart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
  return len;
}




void* __wrap_malloc(uint32_t size){
	return pvPortMalloc(size);
}

void __wrap_free(void* ptr){
	vPortFree(ptr);
}
