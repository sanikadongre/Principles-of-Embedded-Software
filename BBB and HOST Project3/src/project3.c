/*
 * Project3.c
 *
 *  Created on: Dec 2, 2017
 *      Author: monis
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "binary_logger.h"
#include "profiling.h"
#include "uart.h"
#include "System_MKL25Z4.h"
#include "MKL25Z4.h"
#include "rtc.h"
#include "memory.h"
#include "spi.h"
#include "nordic.h"
#include "LED.h"
void profiling_task(void)
{
	uint8_t i=CB_buffer_add_item(&log_queue,PROFILING_STARTED);
	profiling(10);
	return;
}

void spi_task(void)
{
	SPI_write_byte(6);
	uint8_t i = SPI_read_byte();
	if(i==5)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = '5';
	}
	else if(i==6)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = '6';
	}
	else
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = '7';
	}
}


void nordic_task(void)
{

}

void data_analysis_task(void)
{
	uint8_t i=0;
	i=CB_buffer_add_item(&log_queue,DATA_RECEIVED);
	i=CB_buffer_add_item(&log_queue,DATA_ANALYSIS_STARTED);
	i=CB_buffer_add_item(&log_queue,DATA_ALPHA_COUNT);
	i=CB_buffer_add_item(&log_queue,DATA_NUMERIC_COUNT);
	i=CB_buffer_add_item(&log_queue,DATA_PUNCTUATION_COUNT);
	i=CB_buffer_add_item(&log_queue,DATA_MISC_COUNT);
	i=CB_buffer_add_item(&log_queue,DATA_ANALYSIS_COMPLETED);
	return;
}

void project3(void)
{
	uint32_t time_data=0;
	uint8_t logger_id_call=0,flag=0,color=0;
	SystemInit();
	UART_configure();
	log_buffer_configure();
	SysTick_Init();
	SPI_init();
	uint8_t i=0,j=0,peek=0;
	uint8_t* peek_ptr = &peek;
	nrf_write_config();
	nrf_read_config();
	RTC_configure();
	i=CB_buffer_add_item(&log_queue,LOGGER_INITIALIZED);
	i=CB_buffer_add_item(&log_queue,GPIO_INITIALIZED);
	i=CB_buffer_add_item(&log_queue,SYSTEM_INITIALIZED);
	while(1)
	{
		if(receiver_flag==0)
		{
			i = CB_my_peek(&receiver_buffer,0,peek_ptr);
			while((UART0_S1 & UART_S1_TDRE_MASK)==0);
			UART0_D = *peek_ptr;
			receiver_flag=1;
		}
		/*if(peek==65)
		{
			LED_ON();
			LEDFunction(color);
		}
		if(peek==66)
		{
			LED_OFF();
			LEDFunction(color);
		}
		if(peek==67)
		{
			if(color==6)
			{color=0;}
			else
			{color++;}
			LEDFunction(color);
		}*/
		if(peek==68)
		{
			data_analysis_task();
		}

		if(peek==78)
		{
			nordic_task();
		}
		if(peek==80)
		{
			profiling_task();
		}
		if(peek==83)
		{
			spi_task();
		}
		while(CB_is_empty(&log_queue)!= Buffer_Empty)
		{
			i=CB_buffer_remove_item(&log_queue,&logger_id_call);
			log_item(logger_id_call);
		}
		while(CB_is_empty(&buffer)!= Buffer_Empty)
		{
			UART_send(&buffer);
		}
		peek=0x00;
	}
}

