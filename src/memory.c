/*
 * @file memory.c
 * @brief This file contains memory manipulation functions.
 * @author monish and sanika
 * @date Sep 29, 2017
 *
 * memory.c
 *@Long description:-
It has various memory functions like:-
1)my_memmove:- memmove takes 1 byte pointers source and destination and copies a length of bytes from source to destination location without any overalp. If flag =1 , if end of destination adresses intersects at start of source address then we start copying from the begining to handle overlap condition and if the start of destination location intersects at the end of source location then we start copying from the end to handle the overlap condition. If there is no overlap, then flag is zero. Destination is being returned.
2)my_memcpy:- It is used to copy a length of bytes from source to destination. THe copy function may or maynot corrupt the data. Destination is being returned.
3)my memset:- It is used to set all the locations of the source to a given value. It performs the same function as the set_all() function. Source is being returned.
4)my_memzero:- It is used to zero out all the memory.It performs the same function as the clear_all() function. Source is being returned.
5)my_reverse:- It is used for reversing the bytes in a memory. If the length of string is even ( tested by mod operator) the length is divided by 2 to carry out the iterations in the for loop (for reversing) upto half of the length. In case, the length is odd, it is first reduced by 1 and then divided by 2 and then reversing logic is performed. In the reverse for loop, the temp variable stores the source location for every iteration. THen, destination is stored in source and the temp which contains the source is stored back in destination. The function returns source.
6)reserve_words:- Malloc function that contatins the size of operator is used to reserve words in the source. This function returns source.
7)free_words:-  It is used to free dynamic memory location.
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "System_MKL25Z4.h"
#include "MKL25Z4.h"
#include "memory.h"


/***********************************************************************
 * @brief my_memmove()
 * copies a length of bytes from source to destination location without any overalp.
 * @src source pointer
 * @dst destination pointer
 * @length length of data to be transferred
 * @return destination pointer
 ***********************************************************************/
uint8_t* my_memmove(uint8_t* src, uint8_t* dst, uint32_t length)
{ uint8_t flag=0,i;
  if( (src !=NULL) && (dst !=NULL) && (length >=0) )
  {

	for(i=0;i<length;i++)
	{
		if(dst==src+i)
		{
			flag=1;
		}
	}

	for(i=0;i<length;i++)
	{
		if(dst+length-1==src+i) /*For handling overlap condition for every iteration*/
		{
			flag=0;
		}
	}
	if(flag==0)
	{
		for(i=0;i<length;i++)
		{
			*(dst+i)=*(src+i); /*Copies from source to destination for every iteration*/
		}
	}
	if(flag==1)
	{
		for(i=0;i<length;i++)
		{
			*(dst+length-1-i)=*(src+length-i-1); /*Copies from source to destination for every iteration*/
		}
	}

   return dst;
   }
   else if (src == NULL)
   {
   return src;
   }
   else
   {
   return dst;
   }
}

/***********************************************************************
 * @brief my_memcpy()
 * copies a length of bytes from source to destination location and it may or maynot corrupt the data.
 * @src source pointer
 * @dst destination pointer
 * @length length of data to be transferred
 * @return destination pointer
 ***********************************************************************/
uint8_t* my_memcpy(uint8_t* src, uint8_t* dst, uint32_t length)
{   uint8_t i;
    if( (src !=NULL) && (dst !=NULL) && (length >=0) )
    {

	for(i=0;i<length;i++)
	{
		*(dst+i)=*(src+i); /*Copies from source to destination for every iteration*/
	}
      }

	return dst;
}
/***********************************************************************
 * @brief my_memset()
 * sets a value to all the addresses given
 * @src source pointer
 * @value vallue to be set
 * @length length of data to be set
 * @return source pointer
 ***********************************************************************/
uint8_t* my_memset(uint8_t* src, uint32_t length, uint8_t value)
{
	uint8_t i;
        if( (src !=NULL) && (length >=0) )
  {
	for(i=0;i<length;i++)
	{
		*(src+i)=value; /*Sets the char value to the memory location for every iteration*/
	}

  }
	return src;
}

/***********************************************************************
 * @brief my_memzero()
 * initializes 0 to all the addresses given
 * @src source pointer
 * @length length of data to be set 0
 * @return source pointer
 ***********************************************************************/
uint8_t* my_memzero(uint8_t* src, uint32_t length)
{
	uint8_t i;
        if( (src !=NULL) && (length >=0) )
  {
	for(i=0;i<length;i++)
	{
		*(src+i)=0; /*Clears the source location for every iteration*/
	}
  }

	return src;
}

/***********************************************************************
 * @brief my_reverse()
 * reversing the bytes in a memory.
 * @src source pointer
 * @length length of data to be reversed
 * @return source pointer
 ***********************************************************************/
uint8_t* my_reverse(uint8_t* src, uint32_t length) /*Function for reversing bytes*/
{
	int8_t temp;
	size_t i,n;
        if( (src !=NULL) && (length >=0) )
      {
			if(length%2==0) /*If the length is even*/
			{
				n=length/2;
			}
			else
			{
				n=(length-1)/2; /*If the length is odd*/
			}
			for(i=0;i<n;i++) /*Loop for reversing the bytes*/
			{
				 temp = *(src+i);
				 *(src+i) = *(src+length-i-1);
				 *(src+length-i-1) = temp;
			}
       }

	return src;

}

/***********************************************************************
 * @brief reserve_words()
 * allocate memory dynamically in heap
 * @length length of data to be allocated dynamically
 * @return pointer to starting address of the allocated memory
 ***********************************************************************/
int32_t* reserve_words(uint32_t length)
{
  int32_t* src;
  src=malloc(((sizeof(int32_t))*length)); /*To reserve words = length in memory*/
  if (src != NULL)
  {
     return src;
  }
     return src;
}

/***********************************************************************
 * @brief free_words()
 * deallocate memory allocated o a pointer
 * @src pointer whose dynamic memory is to be deallocated
 ***********************************************************************/
void free_words(uint32_t* src) /*Function to free memory*/
{
 if(src != NULL)
  {
    /* Free the dynamic memory allocation by calling free()*/
    free(src);
  }

return;
}
/***************************************************************************
 * @brief memmove_dma
 * Function that is used for moving the data from source to destination using DMA
 * The clock is being enabled for DMA and DMA_MUX
 * Source and destination addresses are being given to Source address and destination address
 * registers respectively
 * overlap is being handled and then data is being transfered for 1, 2 or 4 bytes
 * The interrupt is enabled after transfer and then DMA is started using DMA mask
*****************************************************************************/
void memmove_dma(uint8_t* src, uint8_t* dst, uint32_t length , uint8_t byte)
{
		uint8_t a= 0,b=2,c=1;
		SIM_SCGC7 |= SIM_SCGC7_DMA_MASK; // Clock Gate is being enabled for DMA and DMAMUX
		SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
		DMAMUX0_CHCFG0 &= ~DMAMUX_CHCFG_ENBL_MASK; // Channel 0 is disabled
		DMA_SAR0 = (uint32_t) src;//Source address
		DMA_DAR0 = (uint32_t) dst; //Destination address
		if((src<dst)&&(src+length>dst)) //For handling overlap
		{
			overlap_flag=1; //overlap flag is set
			temp_location =  (uint8_t *)(uint8_t)(uint8_t*) malloc(length); //Saves the location for original destination
			original_length=length;
			original_byte=byte;
			original_dst = dst;
			dst=temp_location;
		}
		else
		{

		}
		if(byte == 4) // For 4 byte transfers
		{
			DMA_DSR_BCR0 |= length;       // Total length of Bytes to transfer
			DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
			DMA_DCR0 |= DMA_DCR_SSIZE(a); //Setting SSIZE mask for 4 Bytes tranfer
			DMA_DCR0 |= DMA_DCR_DINC_MASK;
			DMA_DCR0 |= DMA_DCR_DSIZE(a); //Setting DSIZE mask for 4 Bytes tranfer
		}
		else if(byte == 2) //For 2 byte transferes
		{
			DMA_DSR_BCR0 |= length;
			DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
			DMA_DCR0 |= DMA_DCR_SSIZE(b); //Setting SSIZE mask for 2 Bytes tranfer
			DMA_DCR0 |= DMA_DCR_DINC_MASK;
			DMA_DCR0 |= DMA_DCR_DSIZE(b); //Setting DSIZE mask for 2 Bytes tranfer
		}
		else if(byte == 1) //For 1 byte tranfer
		{
			DMA_DSR_BCR0 |= length;
			DMA_DCR0 |= DMA_DCR_SINC_MASK;
			DMA_DCR0 |= DMA_DCR_SSIZE(c); //Setting SSIZE mask for 1 Byte tranfer
			DMA_DCR0 |= DMA_DCR_DINC_MASK;
			DMA_DCR0 |= DMA_DCR_DSIZE(c); //Setting SSIZE mask for 1 Byte tranfer

		}
		DMA_DCR0 |= DMA_DCR_EINT_MASK; //Enables the Interrupt
		NVIC_EnableIRQ(DMA0_IRQn);
		__enable_irq;
		DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK; //Enable channel 0
		DMA_DCR0 |= DMA_DCR_START_MASK;           // DMA start mask
		/*while(DMA_DSR_BCR0 != DMA_DSR_BCR_DONE_MASK);
		{
			DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
		}*/
	}

/***************************************************************************
 * @brief memmset_dma
 * Function that is used for setting value to the source using dma
 * The clock is being enabled for DMA and DMA_MUX
 * Source and destination addresses are being given to Source address and destination address
 * registers respectively
 * memset is being done for 1 or 2 or 4 bytes
 * Interrupt is enabled after completion of memset
 * The channel 0 is enabled
 * The DMA is being started using the DMA start mask
*****************************************************************************/

void memset_dma(uint8_t* dst, uint32_t length, uint32_t value, uint8_t byte)
{

	uint8_t a= 0,b=2,c=1;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK; // Clock Gate is being enabled for DMA and DMAMUX
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	DMAMUX0_CHCFG0 &= ~DMAMUX_CHCFG_ENBL_MASK; // Channel 0 is disabled
	DMA_SAR0 = (uint32_t) &value; //address of value to be set to the SAR register
	DMA_DAR0 = (uint32_t) dst;    //address of destination to be given to the DAR register

	if(byte == 4)                //For 4 bytes transfer
	{
		DMA_DSR_BCR0 |= length;  // Total length of bytes to transfer
		DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
		DMA_DCR0 |= DMA_DCR_SSIZE(a); //Setting SSIZE for 4 bytes transfer
		DMA_DCR0 |= DMA_DCR_DINC_MASK;
		DMA_DCR0 |= DMA_DCR_DSIZE(a); //Setting DSIZE for 4 bytes transfer
	}
	else if(byte == 2)           //For 2 bytes transfer
	{
		DMA_DSR_BCR0 |= length; // Total length of bytes to transfer
		DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
		DMA_DCR0 |= DMA_DCR_SSIZE(b); //Setting SSIZE for 2 bytes transfer
		DMA_DCR0 |= DMA_DCR_DINC_MASK;
		DMA_DCR0 |= DMA_DCR_DSIZE(b); //Setting DSIZE for 2 bytes transfer
	}
	else if(byte==1)           //For 1 byte transfer
	{
		DMA_DSR_BCR0 |= length;   //Total length of bytes to be transfered
		DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
		DMA_DCR0 |= DMA_DCR_SSIZE(c); //Setting SSIZE for 1 byte transfer
		DMA_DCR0 |= DMA_DCR_DINC_MASK;
		DMA_DCR0 |= DMA_DCR_DSIZE(c); //Setting DSIZE for 1 byte tranfer

	}
	DMA_DCR0 |= DMA_DCR_EINT_MASK; //Enable DMA interrupt mask
	NVIC_EnableIRQ(DMA0_IRQn);
	__enable_irq;
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK; //Enable Channel 0
	DMA_DCR0 |= DMA_DCR_START_MASK; //DMA start mask
	/*while(DMA_DSR_BCR0 != DMA_DSR_BCR_DONE_MASK);
	{
		DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
	}*/
}
/**********************************************************************
 * @brief DMA_overalp_handler
 * Function that handles the overlap in case of DMA transfer
 * The overflow flag is zero
 * memmove is being called to handle the overlap
 *
 ************************************************************************/
void DMA_overlap_handler(void)
{
	overlap_flag=0;
	memmove_dma(temp_location, original_dst, original_length , original_byte);
}
