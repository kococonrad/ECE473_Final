/*
 * SPI.h
 *
 * Created: 4/16/2016 8:08:45 PM
 *  Author: gbone
 */ 
#ifndef SPI_H_
#define SPI_H_

//SPI Decleration
#define SS 0
#define SCK 1
#define MOSI 2
#define MISO 3

//SPI Project Declerations
#define BG_OEPort PORTB
#define BG_OEPortDir DDRE
#define BG_OEPin PE5 //PB7
#define BG_SRCLK PB1 //PB1
#define BG_REGCLKPort PORTD
#define BG_REGCLKDir DDRD
#define BG_REGCLK PD2 //PD2



void init_SPI(void);
char SPI_send(char chr);
void select(uint8_t slvdevice);
void deselect(uint8_t slvdevice);
int spi_writeRegs(unsigned char sel, unsigned char reg_addr,
unsigned char length, unsigned char const *data);
unsigned int spi_writeReg(unsigned char sel, unsigned char reg_addr, unsigned char data);
void spi_readRegs(unsigned char sel, unsigned char reg_addr,
unsigned char length, unsigned char *data);
inline static uint8_t transfer(uint8_t data){
	SPDR=data;
	
	asm volatile("nop");
	while(!(SPSR & _BV(SPIF)));
	return SPDR;
}
#endif /* SPI_H_ */