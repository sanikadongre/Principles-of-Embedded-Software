/*
* spi.h
*
*
* Created on: Nov 19, 2017
*
* Author: Sanika
*/

#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>

void SPI_init();
uint8_t SPI_state();
void SPI_write_byte( uint8_t byte);
void SPI_send_packet( uint8_t* p, uint8_t length);
uint8_t SPI_read_byte();
void SPI_flush();


#endif


