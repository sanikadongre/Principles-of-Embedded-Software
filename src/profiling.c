/*
 * profiling.c
 *
 *  Created on: Nov 30, 2017
 *      Author: monish
 */
#include "memory.h"
#include <stdint.h>
#include "System_MKL25Z4.h"
#include "MKL25Z4.h"
#include <stdlib.h>
#include <string.h>
#include "binary_logger.h"
#include "profiling.h"

/**********************************************************************
 * @brief SysTick_Init()
 * It is used to setup TDM
 * VAL is initialized to 0
 * LOAD is set to 0xFFFFFF
 * CTRL register is set to value 5 to enable CTRL and CLK source
 ***********************************************************************/

void SysTick_Init() //Setup TDM
{
	SysTick-> LOAD = SysTick_LOAD_RELOAD_Msk;        // LOAD register Set to 0xFFFFFF
	SysTick-> VAL = 0;                                // VAL initialized to 0
	SysTick-> CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk); // CTRL register Set to 5
	return;
}

/*********************************************************************
* @brief Time calculation function
*  It has time_start and time_end function
* pre_interrupt uses SysTick that has a LOAD pointer
* post_interrupt uses SysTick that has a LOAD pointer
* profiling time is the difference between pre and post interrupt
* returns profiling_time
***********************************************************************/

void time_start(uint8_t state)
{
	     pre_interrupt[state] = SysTick-> VAL; //SysTick points to VAL for pre_interrupt
	     return;
}

void time_end(uint8_t state)

{
	post_interrupt[state] = SysTick-> VAL;  //SysTick points to VAL for post_interrupt
	profiling_time[state] = pre_interrupt[state] - post_interrupt[state]; // profiling_time is the difference between pre_interrupt and post interrupt
	return;
}
/*******************************************************************************
 * @brief log_result(uint32_t length, uint8_t k)
 * Function that displays the log result
 ********************************************************************************/

void log_result(uint32_t length, uint8_t k)
{
		uint8_t i=0,j=0,count=0;
		uint8_t array[20];
		uint8_t* value = array;
		uint8_t* profiling_time_ptr = (uint8_t*)&profiling_time[k];
		j = my_itoa(k,value,10);
		for(i=0;i<j;i++)
		{
			CB_buffer_add_item(&log_profiling_result,*(value+i));
			count += get_count_of_1(*(value+i));
		}
		j = my_itoa(length,value,10);
		for(i=0;i<j;i++)
		{
			CB_buffer_add_item(&log_profiling_result,*(value+i));
			count += get_count_of_1(*(value+i));
		}
		j = my_itoa(profiling_time[k],value,10);
		for(i=0;i<j;i++)
		{
			CB_buffer_add_item(&log_profiling_result,*(value+i));
			count += get_count_of_1(*(value+i));
		}
		return;
}
/**********************************************************************
* Profiling function that is used for memset and memmove function
* It calls standard C version
* It calls memory function
* It calls memmory function using Optimized O3 flag
* It calls memset and memmove by using dma
* Free destination and free source
***********************************************************************/

void profiling(uint32_t length)
{
	uint32_t length_temp=0;
	uint8_t i=0,flag_length_change=0,j=0,k=0;
	uint8_t* dst = (uint8_t*) malloc(length+10); //memory allocation to dst
	uint8_t* src = dst + 10; //memory allocation to src
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		flag_length_change=1;
		length_temp=length;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		src=memset((void*)src,(uint8_t)(k+1),4*length); //using standary C version memset
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//1
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		src=memmove((void*)dst,(void*)src,4*length); //using standard C version memmove
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//2
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		src = my_memset(src, length, (uint8_t)k); //using memory function for memset
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//3
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		dst = my_memmove( src, dst, length);          //using memory function for memmove
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//4
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		src = my_memset(src, length, (uint8_t) k); //using memory function for memset by O3 flag
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//5
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		dst = my_memmove( src, dst, length);           //using memory function for memmove by O3 flag
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//6
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		memset_dma(src,length,k,1);                 //using memset function by dma
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//7
	k++;
	time_start(k);
	if(length>100)
	{
		loop_for_big_memory = length/100 + 1;
		length=100;
	}
	for(i=0;i<loop_for_big_memory;i++)
	{
		memmove_dma(src,dst,length,1);   // using memmove function by dma
	}
	if (flag_length_change==1)
	{
		length=length_temp;
	}
	time_end(k);
	log_result(length,k);//8
	k++;
	free_words((uint32_t*)dst);                          //frees destination
	free_words((uint32_t*)src);                          //frees source
	return;
}
/********************************************************************
 * @brief DMA_IRQHandler()
 * Function checks for overlap and checks if the data is being transmitted
 * If the transfer is complete, it sends Profiling completed and profiling result
 */

void DMA0_IRQHandler(void)
{
		static uint8_t DMA_transfers_counter = 0,i=0;
		uint8_t counter;
		DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;  //Checks if transfer is complete
		if(overlap_flag == 0)
		{
			DMA_transfers_counter++;            //increments the DMA_transfer_counter
		}
		else
		{
			DMA_overlap_handler();             //Calls the DMA_overlap_handler()
		}
		if(DMA_transfers_counter == 2)
		{
			i=CB_buffer_add_item(&log_queue,PROFILING_COMPLETED);    //Sends log for Profiling Complete
			i=CB_buffer_add_item(&log_queue,PROFILING_RESULT);       //Sends log for Profiling Result
			DMA_transfers_counter = 0;
		}
}
