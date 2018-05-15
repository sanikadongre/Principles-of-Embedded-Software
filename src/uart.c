/*
 * uart.c
 *
 *  Created on: Oct 27, 2017
 *      Author: monish
 *
 * @file uart.c
 * @brief This file contains circular buffer operation functions.
 * @author monish and sanika
 * @date oct 25s, 2017
 *
 * uart.c
 * @Long description:-
 */

#include <stdint.h>
#include "system_MKL25Z4.h"
#include "MKL25Z4.h"
#include "uart.h"
#include "conversion.h"
#include "circbuf.h"
#define baud_rate  57600
#define size 1000
uint8_t alpha=0,nums=0,punc=0,misc=0;

void sort(uint8_t peek)
{
	if(((peek>=65)&&(peek<=90))||((peek>=97)&&(peek<=122)))
		alpha++;
	else if(((peek>=48)&&(peek<=57)))
		nums++;
	else if(((peek>=33)&&(peek<=47))||((peek>=91)&&(peek<=96))||((peek>=123)&&(peek<=127)))
		punc++;
	else if(!((peek==0x0D)||(peek==0x1B)||(peek==0x0A)||(peek==0x00)))
		misc++;
	return;
}

void char_count_display(void)
{
	char stringc[]="\n\r counts of characters",stringa[]="\n\r number of alphabets=",stringp[]="\n\r number of punctuations=",stringm[]="\n\r number of miscellaneous=",stringn[]="\n\r numbers=";
	uint8_t i=0,j=0,n=11,a=23,m=27,p=26,c=23,array[20];
	uint8_t* value = array;
	UART0->C2 &= ~UART_C2_RIE_MASK;
	for(i=0;i<c;i++)
	{
		j=(uint8_t)stringc[i];
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = j;
	}
	for(i=0;i<a;i++)
	{
		j=(uint8_t)stringa[i];
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = j;
	}
	j= my_itoa(alpha,value,10);
	for(i=0;i<j-1;i++)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = *(value+i);
	}
	for(i=0;i<n;i++)
	{
		j=(uint8_t)stringn[i];
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = j;
	}
	j= my_itoa(nums,value,10);
	for(i=0;i<j-1;i++)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = *(value+i);
	}
	for(i=0;i<p;i++)
	{
		j=(uint8_t)stringp[i];
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = j;
	}
	j= my_itoa(punc,value,10);
	for(i=0;i<j-1;i++)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = *(value+i);
	}
	for(i=0;i<m;i++)
	{
		j=(uint8_t)stringm[i];
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = j;
	}
	j= my_itoa(misc,value,10);
	for(i=0;i<j-1;i++)
	{
		while((UART0_S1 & UART_S1_TDRE_MASK)==0);
		UART0_D = *(value+i);
	}
	while((UART0_S1 & UART_S1_TDRE_MASK)==0);
	UART0_D = 10;
	while((UART0_S1 & UART_S1_TDRE_MASK)==0);
	UART0_D = 13;
	UART0->C2 |= UART_C2_RIE_MASK;
}

void get_char_count(uint8_t* alpha_ptr,uint8_t* nums_ptr,uint8_t* punc_ptr,uint8_t* misc_ptr)
{
	*(alpha_ptr)=alpha;
	*(nums_ptr)=nums;
	*(punc_ptr)=punc;
	*(misc_ptr)=misc;
	return;
}

uint8_t get_alpha_count(void)
{
	return alpha;
}

uint8_t get_nums_count(void)
{
	return nums;
}

uint8_t get_punc_count(void)
{
	return punc;
}

uint8_t get_misc_count(void)
{
	return misc;
}

/***********************************************************************
 * @brief UART_configure()
 * This function configures the UART
 ***********************************************************************/
void UART_configure(void)
{
	uint8_t i;
	uint16_t baud = (uint16_t)(SystemCoreClock/(baud_rate*16));
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
	PORTA_PCR1 = PORT_PCR_MUX(2);
	PORTA_PCR2 = PORT_PCR_MUX(2);
	SIM_SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL(1);
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
	UART0->S2 = 0X00;
	UART0->C1 = 0X00;
	NVIC_EnableIRQ(UART0_IRQn);
	UART0->C2 |= (UART0_C2_RE_MASK|UART0_C2_TE_MASK);
	UART0->C2 |= UART_C2_RIE_MASK;
	UART0->C3 = 0X00;
	UART0->C5 = 0X00;
	UART0_BDL |= (uint8_t)(baud & UART0_BDL_SBR_MASK);
	UART0_BDH |= (uint8_t)((baud >>8 ) & UART0_BDH_SBR_MASK);
	i = CB_init(&buffer,size);
	buffer.head = buffer.buffptr;
	buffer.tail = buffer.buffptr;
	buffer.count = 0;
	buffer.length = size;
	buffer.count = 0;
	i = CB_init(&receiver_buffer,100);
	receiver_buffer.head = receiver_buffer.buffptr;
	receiver_buffer.tail = receiver_buffer.buffptr;
	receiver_buffer.count = 0;
	receiver_buffer.length = 100;
	return;
}

/***********************************************************************
 * @brief UART_send()
 * This function stores the value to be transmitted in transmit circular buffer
 * @tx_circbuf pointer to circular buffer
 ***********************************************************************/
void UART_send (CB_t* tx_circbuf)
{
uint8_t* temp;
CB_buffer_remove_item(tx_circbuf,temp);
sort(*temp);
while((UART0_S1 & UART_S1_TDRE_MASK)==0);
UART0_D = *temp;
return;
}

/*void UART_send_test(void)
{
	uint8_t i = 65;
	while((UART0_S1 & UART_S1_TDRE_MASK)==0);
	/* Send the character
	UART0_D = i;
return;
}*/

/***********************************************************************
 * @brief UART_send_n()
 * This function stores values from a certain memory location to be transmitted in transmit circular buffer
 * @tx_circbuf pointer to circular buffer
 * @length length of data
 ***********************************************************************/
void UART_send_n(CB_t* tx_circbuf,uint8_t length)
{
uint8_t temp,j;
uint8_t* temp_ptr=&temp;
uint8_t i;
for(i=0;i<length;i++)
{
	j = CB_buffer_remove_item(tx_circbuf,temp_ptr);
	sort(*temp_ptr);
	while((UART0_S1 & UART_S1_TDRE_MASK)==0);
	UART0_D = *temp_ptr;
}

return;
}


/***********************************************************************
 * @brief UART_receive()
 * This function stores a from receive circular buffer to  a certain memory location
 * @rx_circbuf pointer to circular buffer
 ***********************************************************************/
uint8_t UART_receive(CB_t* rx_circbuf)
{
uint8_t i,temp;
temp = UART0_D;
i = CB_buffer_add_item(rx_circbuf,temp);
return Success;
}
/***********************************************************************
 * @brief UART_receive_n()
 * This function stores multipl000e values from receive circular buffer to  a certain memory location
 * @rx_circbufpointer to circular buffer
 * @length length of data
 ***********************************************************************/
uint8_t UART_receive_n(CB_t* cbptr,uint8_t length)
{
uint8_t temp,i;
for(i=0;i<length;i++)
{
	while((UART0_S1 & UART_S1_RDRF_MASK)==0);
	temp = UART0_D;
	CB_buffer_add_item(cbptr,temp);
}
return Success;
}

/***********************************************************************
 * @brief UART0_IRQHandler()
 * This function checks the register values after interrupt occurs and sends or receives data by UART
 ***********************************************************************/
void UART0_IRQHandler(void)
{
 uint8_t i=UART_receive(&receiver_buffer);
 receiver_flag=0;
}


