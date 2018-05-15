
#include "MKL25Z4.h"
#include "nordic.h"
#include "spi.h"
#include "gpio.h"

/***********************************************************************
 * @brief nrf_read_register
 * It reads the SPI_write_byte register in a variable and returns the value
 ***********************************************************************/

void nrf_read_register(uint8_t register_function)

{
   SPI_write_byte(0x00|register_function);//Reads the command

}

/***********************************************************************
 * @brief nrf_write_register
 * It sets the SPI_write_byte register to a value
 ***********************************************************************/

void nrf_write_register(uint8_t register_function)
{
  SPI_write_byte(0x25|register_function); //Writes the command
}

/***********************************************************************
 * @brief nrf_read_status
 * It is a function to read the nordic status register
 * SPI_write_byte has been set to FF and a variable is used to store the result
 * for SPI_read
 ***********************************************************************/

void nrf_read_status() //function that reads the nrf status register
{
	uint8_t f;
	nrf_chip_enable(); //Chip select is made low
	nrf_read_register(NRF_STATUS_REG);
    SPI_write_byte(0xFF);
	f= SPI_read_byte();
	nrf_chip_disable(); //Chip select is set to high

}

/***********************************************************************
 * @brief nrf_write_config
 * Chip select is low
 * It is a function to write command to the Config register
 * Data is written to config register and that data is being stored in
 * config variable. The chip select is then set to high
 ***********************************************************************/

 void nrf_write_config() //function that is used for writing config register
{
	 nrf_chip_enable();
    nrf_write_register(NRF_CONFIG_REG); //Write command to the Config register
    SPI_write_byte(0x06);//Write data to  the Config register
    nrf_chip_disable();
}

/***********************************************************************
 * @brief nrf_read_config
 * The Clock to Port B is being enabled.
 * Chip select is low
 * It is a function to read command to the Config register
 * A dummy value is sent to receive data
 * The chip select is then set to high and the dummy value is being returned
 ***********************************************************************/
 void nrf_read_config()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //enable clock to port C
	uint8_t j;
	nrf_chip_enable(); //Chip select  is made low
    nrf_read_register(NRF_CONFIG_REG); // Read command to the Config register
	j=SPI_write_byte(0X3F);//Sends a test value to see if j returns the value that is being written to the nrf_write_config
	nrf_chip_disable(); //Chip select is set to high

}
/***********************************************************************
 * @brief nrf_read_rf_setup
 * Function to read the rf setup register
 * Chip select is low
 * nrf-write-register is used for NORDIC_RF_SETUP_REG
 * A dummy value is sent to receive data
 * The chip select is then set to high
 ***********************************************************************/

void nrf_read_rf_setup() //Read the rf_setup register
{
	uint8_t h;
	nrf_chip_enable();
	nrf_write_register(NRF_RF_SETUP_REG);
	h=SPI_write_byte(0x32);
	nrf_chip_disable();

}
/***********************************************************************
 * @brief nrf_write_rf_setup
 * Function to write the rf setup register
 * Chip select is low
 * nrf-write-register is used for NORDIC_RF_SETUP_REG
 * A value is written
 * The chip select is then set to high
 ***********************************************************************/

void nrf_write_rf_setup() //Setup the rf register
{
	nrf_chip_enable();
    nrf_write_register(NRF_RF_SETUP_REG);
    SPI_write_byte(0x02);
    nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_read_rf_ch
 * Function to read the ch register
 * Chip select is low
 * nrf-write-register is used for NORDIC_RF_CH_REG
 * A value is written
 * The chip select is then set to high
 * The fifo status is then read and returned
 ***********************************************************************/

 void nrf_read_rf_ch() //Read rf_ch register
{
	 uint8_t i;
	 nrf_chip_enable();
	 nrf_write_register(NRF_RF_CH_REG);
	 i = SPI_write_byte(0x7F);
	 nrf_chip_disable();
	 nrf_read_fifo_status(); //Read the fifo status
}
/***********************************************************************
 * @brief nrf_write_rf_ch
 * Function to write to the ch register
 * Chip select is low
 * nrf-write-register is used for NORDIC_RF_CH_REG
 * values are written on the channel
 * The chip select is then set to high
 ***********************************************************************/

void nrf_write_rf_ch() //Write command to rf_ch register
{
	uint32_t i;
	uint8_t channel;
	nrf_chip_enable();
	nrf_write_register(NRF_RF_CH_REG);
	for(i=0; i<5; i++)
	{
		channel = SPI_write_byte(0x10);
	}
	nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_read_TX_addr
 * Function to read the TX_ADDR
 * Chip select is low
 * nrf-read-register is used for reading the TX_ADDR register
 * values are written using SPI_write_byte
 * The chip select is then set to high
 ***********************************************************************/

void nrf_read_TX_ADDR() //Read the tx address register
{
	uint32_t i;
	uint8_t k;
	nrf_chip_enable();
	nrf_read_register(TX_ADDR); //Command to read the TX_ADDR register
	 for(i=0;i<5;i++)
	 {

		 k=SPI_write_byte(0xFF);
	 }
	 nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_write_TX_ADDR
 * Function to write to the tx address register
 * Chip select is low
 * nrf_write_register is used for TX_ADDR register
 * values are written using SPI_write_byte
 * The chip select is then set to high
 ***********************************************************************/

void nrf_write_TX_ADDR() // Command to write to the tx address register
{
	uint32_t i;
	nrf_chip_enable();
    nrf_write_register(TX_ADDR);
    for(i=0;i<5;i++)
	 {
	   SPI_write_byte(0xBB);// Setting BB as the TX address
	 }
    nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_read_fifo_status
 * Function to read the nrf_fifo_status
 * Chip select is low
 * nrf_read_register is used for FIFO_STATUS_REG
 * values are written
 * The chip select is then set to high
 ***********************************************************************/

void nrf_read_fifo_status() //Function to read the nrf fifo_status_register
{

	uint8_t m;
	nrf_chip_enable();
    nrf_read_register(FIFO_STATUS_REG); //Command to read the fifo_status_register
	m=SPI_write_byte(0xFF);
	nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_flush_tx_fifo
 * Function to flush_tx_fifo
 * Chip select is low
 * SPI_write_byte sending 0xE1
 * The chip select is then set to high
 ***********************************************************************/

void nrf_flush_tx_fifo() //Function that flushes the tx_fifo register
{
	nrf_chip_enable();
	SPI_write_byte(0xE1);
	nrf_chip_disable();
}
/***********************************************************************
 * @brief nrf_flush_rx_fifo
 * Function to flush_rx_fifo
 * Chip select is low
 * SPI_write_byte sending 0xE2
 * The chip select is then set to high
 ***********************************************************************/

void nrf_flush_rx_fifo() //Function that flushes the rx_fifo register
{
	nrf_chip_enable();
	SPI_write_byte(0xE2);
	nrf_chip_disable();
}



