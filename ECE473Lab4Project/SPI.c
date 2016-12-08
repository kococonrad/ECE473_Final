#ifndef F_CPU
# define F_CPU 16000000UL
#endif
#include<avr/io.h>
#include<util/delay.h>
#include "SPI.h"
#include<avr/interrupt.h>
#include<string.h>

#define readbit 0x80

void init_SPI(void)
{
	DDRB |= (1<<SS)|(1<<SCK)|(1<<MOSI)|(1<<MISO);//Turn on SS, MOSI, SCLK
	SPCR  |=   (1<<SPE) | (1<<MSTR);  //set up SPI mode
	SPSR  |=   (1<<SPI2X);           // double speed operation
}//end init_SPI()

char SPI_send(char chr)
{
	char receivedchar=0;
	SPDR=chr;
	while(bit_is_clear(SPSR,SPIF)){}
	receivedchar=SPDR;
	return (receivedchar);
}//End SPI_send

void select(uint8_t slvdevice){
	switch (slvdevice)
	{
		default:
		break;
	}
	return;
}

void deselect(uint8_t slvdevice)
{
	switch (slvdevice)
	{
		default:
		break;
	}
	return;
}

int SPI_writeRegs(unsigned char sel, unsigned char reg_addr,
unsigned char length, unsigned char const *data) {
	uint8_t i;
	select(sel);
	
	SPDR=reg_addr;
	while(bit_is_clear(SPSR,SPIF)){}
	for(i=0; i<length; i++)
	{
		SPDR=data[i];
		while(bit_is_clear(SPSR,SPIF)){};
		SPDR=0xFF;
	}
	deselect(sel);
	_delay_us(10);
	return 0;
}

unsigned int spi_writeReg(unsigned char sel, unsigned char reg_addr, unsigned char data){
	unsigned int temp_val;
	select(sel);
	transfer(reg_addr);
	temp_val=transfer(data);
	deselect(sel);
	_delay_us(50);
	return temp_val;
}

void spi_readRegs(unsigned char sel, unsigned char reg_addr,
unsigned char length, unsigned char *data) {
	unsigned int  i = 0;
	
	select(sel);
	transfer(reg_addr | readbit);
	for(i=0; i<length; i++) data[i] = transfer(0x00);
	deselect(sel);
	_delay_us(50);
}
