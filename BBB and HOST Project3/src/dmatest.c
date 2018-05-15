

#include "MKL25Z4.h"
#include "memory.h"
#include <stdint.h>
#include <stdlib.h>

void main()
{

uint8_t* src;
uint8_t* dst;
uint8_t sum=0, sum1=0;
uint8_t i=0,m[4]={1,2,3,4};
uint8_t n[4]={5,6,7,8};
uint32_t length=4;
src = m;
dst = n;
uint32_t value = 4;

memset_dma(src,length,value,4);
for(i=0;i<length;i++)
{
	sum+= m[i];
}
memmove_dma(src,dst,length,4);
for(i=0;i<length;i++)
{
	 n[i];
}
}
