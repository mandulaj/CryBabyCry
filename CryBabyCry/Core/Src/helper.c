

#include "helper.h"
#include <stdio.h>
#include "stm32l4xx_hal.h"
#include <stdarg.h>


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
		USART1_printf("%f ", buf[i]);
	}

}

void print_buffer_q31(q31_t *buf, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		USART1_printf("%ld ", buf[i]);
	}

}

void print_buffer_q15(q15_t *buf, uint32_t len){
	for (uint32_t i = 0; i < len; i++){
		USART1_printf("%i ", buf[i]);
	}

}


#define PRINT_LEN 1024
static char USART_print_buf[PRINT_LEN+1];

void USART1_printf(char *fmt, ...){
	va_list arg_ptr;

	va_start(arg_ptr, fmt);

	uint32_t len = vsnprintf(USART_print_buf, PRINT_LEN+1, fmt, arg_ptr);

	HAL_USART_Transmit(&husart1, (uint8_t *) USART_print_buf, len, HAL_MAX_DELAY);


	va_end(arg_ptr);

}

//
//int _write(int fd, char * ptr, int len)
//{
//  HAL_USART_Transmit(&husart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
//  return len;
//}


//
//
//#ifdef __GNUC__
///* With GCC/RAISONANCE, small msg_info (option LD Linker->Libraries->Small msg_info
//   set to 'Yes') calls __io_putchar() */
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#define GETCHAR_PROTOTYPE int __io_getchar(void)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
//#endif /* __GNUC__ */
//
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the serial port and Loop until the end of transmission */
//  while (HAL_OK != HAL_USART_Transmit(&husart1, (uint8_t *) &ch, 1, 30000))
//  {
//    ;
//  }
//  return ch;
//}
//
///**
//  * @brief Retargets the C library scanf function to the USART.
//  * @param None
//  * @retval None
//  */
//GETCHAR_PROTOTYPE
//{
//  /* Place your implementation of fgetc here */
//  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
//  uint8_t ch = 0;
//  while (HAL_OK != HAL_USART_Receive(&husart1, (uint8_t *)&ch, 1, 30000))
//  {
//    ;
//  }
//  return ch;
//}
