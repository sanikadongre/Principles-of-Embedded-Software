#include "MKL25Z4.h"
#include "nordic.h"


 void GPIO_nrf_configure()
 {
		SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;// Clock to PORT C
		PORTC->PCR[4] = PORT_PCR_MUX(1);   //Using Gpio for selecting chip
		PORTC->PCR[3] = PORT_PCR_MUX(1);
		GPIOC->PDDR |= 1<<4;
		GPIOC->PSOR |= 1<<4;
		GPIOC->PDDR |= 1<<3;
		GPIOC->PSOR |= 1<<3;

 }


