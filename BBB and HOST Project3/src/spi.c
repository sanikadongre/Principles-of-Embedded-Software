



#include "MKL25Z4.h"
#include "SPI.h"
#include "gpio.h"

void SPI_init(void) {
	SPI_configure();
	SPI0->C1 = 0x50;// Enable SPI and set as Master
	SPI0_BR = (SPI_BR_SPPR(0x03) | SPI_BR_SPR(0x08));     //Set baud rate prescale divisor to 3 & set baud rate divisor to 512 for baud rate of 15625 hz
}

uint8_t SPI_state(void) { //Shows the status of SPI
	return SPI0->S;
}

// Write out all characters in supplied buffer to register at address
uint8_t SPI_write_byte(uint8_t byte)
{
        while ((SPI_state() & 0x20) != 0x20); //Check if transfer buffer is empty
		SPI0->D = byte;//Send data
		while ((SPI_state() & 0x80) != 0x80); //Check if transfer is complete, hence recieve buffer full
		return SPI0->D; //return data
}
void SPI_send_packet(uint8_t* p, uint8_t length) {
	uint8_t i=0;
	for (i = 0; i < length; i++) {
		while ((SPI_state() & 0x20) != 0x20);
		SPI0->D = *(p+i);
	}
}

uint8_t SPI_read_byte()
{
	uint8_t byte;
	SPI0->D = 0x00;
	while ((SPI_state() & 0x80) != 0x80);
	byte = SPI0->D;
	return byte;
}

void spi_flush() //To flush out the transmit and recieve buffers
{
	SPI0_C1&=0xBF;
	SPI_init();
}
