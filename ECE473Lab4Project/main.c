/*
 * ECE473Lab4.c
 *
 * Created: 11/2/2016 9:06:00 PM
 * Author : Kody Conrad
 *
	Count controlled via two rotary encoders

	Mode visualized via LED bargraph

	Mode control via Button board

	SW1 = Mode1 = increment 1; SW2 = Mode2 = increment 2; SW3 = Mode3 = increment 4
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<math.h>
#include<stdlib.h>
#include "SPI.h"
#include "RTC_Time.h"
#include "SegmentDisplay.h"
#include "WaveGeneration.h"
#include "Encoder.h"
#include "main.h"
#include "LCDDriver.h"
#include "twi_master.h"
#include "uart_functions.h"
#include "si4734.h"



#define DEBOUNCE_TIME 3 	// Number of button reads until it acknowledge button press

uint8_t si4734_wr_buf[9];//not sure we ever use 9 on either of these
uint8_t si4734_rd_buf[9];
uint8_t si4734_tune_status_buf[8];

volatile uint8_t system_status;
volatile uint16_t eeprom_fm_freq;
volatile uint16_t eeprom_am_freq;
volatile uint16_t eeprom_sw_freq;
volatile uint8_t  eeprom_volume;

volatile uint16_t current_fm_freq = 10110;
volatile uint16_t current_am_freq;
volatile uint16_t current_sw_freq;
volatile uint16_t radio_volume;
volatile uint8_t  current_volume = 0;
volatile uint8_t STC_interrupt;

volatile int count = 0;
volatile uint8_t brightness=0;

uint8_t CurMode = 0x01;
uint8_t frequency = 0;
static uint8_t lastButton = 0, deb_count = 0;

char ActiveModes = 0x00;
char ColonControl = 0x00;
RTC_Time Time;

RTC_Time AlarmTime;

uint8_t AlarmArmed = 0;
uint8_t data[2] = {0,0};
int temp = 0;
int exttemp = 0;
char extTempArr[2] = {0,0};

int scale[] = {C5, CSharp5, D5, DSharp5, E5, F5, FSharp5, G5, GSharp5, A5, ASharp5, B5};


//******************************************************************************
// External interrupt 7 is on Port E bit 7. The interrupt is triggered on the
// rising edge of Port E bit 7.  The i/o clock must be running to detect the
// edge (not asynchronouslly triggered)
//******************************************************************************
ISR(INT7_vect){STC_interrupt = TRUE;}
/***********************************************************************/

ISR(TIMER0_OVF_vect){
	if (frequency > 11)
	{
		frequency = 0;	
	}
	else if (frequency < 0)
	{
		frequency = 0;
	}
	ColonControl ^= 0x01;
	incrementTime(&Time);
	extTempArr[0] = uart_getc();
	extTempArr[1] = uart_getc();
	
	//setFrequency(scale[frequency]);
	//frequency++;
	count++;
}

ISR(ADC_vect){
	setBrightness(ADCH);
}

//Input: NULL
//Ouput: Returns the state of the pushbuttons encompassing debounce functionality
uint8_t readButtons(){
	uint8_t result;
	DDRA = 0x00; // Set port A for input

	DigitSelectPort = BUTTON_EN; // Enables only the pushbuttons disabling 7segment
	PORTA = 0xFF; // Enable pullup resistors on pushbuttons; active low
	result = PINA ^ 0xFF;
	_delay_us(2); // Delay for reading PIN

	//Debounce buttons on PORTA
	if(result != lastButton){
		lastButton = result;
		deb_count = 0;
		return 0x00;
	}
	if(result == lastButton && deb_count <= DEBOUNCE_TIME) deb_count++; //if the result equals the previous pushed button it iterates the debounceCnt without risk of rolling over
	return (deb_count == DEBOUNCE_TIME) ? result : 0x00; // returns result if debounce is true otherwise returns no pushed Buttons
}

void Timer0Setup(){
	ASSR = (1 << AS0);
	TCNT0 = 0x00;
	TCCR0 = (1<<CS00)|(1<<CS02); //Prescaling of 128 for real time clock
	//TCCR0 = (0<<CS00)|(1<<CS01); //x4 Clock speed
	TIMSK = (1<<TOIE0); //Timer 0 Overflow flag interrupt enable
	_delay_ms(1000); //Allow crystal stabilization
}

void init_DeviceDependencies(){
	pinOutput(BG_OEPortDir, BG_OEPin);
	pinOutput(BG_REGCLKDir, BG_REGCLK);
	ENC_DDR  |= (1<<ENC_LoadPin)|(1<<ENC_EnPin);
	ENC_PORT |= (1<<ENC_LoadPin)|(1<<ENC_EnPin);
	DigitSelectDDR |= DECODER_Mask; //Initilization for DDRC segment display decoder.
}

void write2Bar(uint8_t val){
	SPDR = val; //Initiates spi transfer
	//waits till done (8 cycles) SPIF (SPI Interrupt Flag) in SPSR Register (SPI Status Register)
	while(bit_is_clear(SPSR, SPIF));

	//Updates bargraph with new values
	PORTD |= 0x04;
	PORTD &= ~0x04;
}

void getMode(){
	uint8_t modetemp= readButtons();
	
	char tempin[7];
	char tempout[7];
	switch (modetemp)
	{
		case CLOCK_SET:
			write2Bar(CLOCK_SET);
			setHour(&Time);
			setMin(&Time);
		break;
		case ALARM_SET:
			write2Bar(ALARM_SET);
			setHour(&AlarmTime);
			setMin(&AlarmTime);
			TOGGLE_BIT(system_status, ALARM_ON);
		break;
		case RADIO_MODE:
			mute_radio();
		break;
		case RADIO_TOGGLE:
			mute_radio();
			//MuteAll();
			if (radio_volume == 0)
			{
				CLEAR_BIT(system_status, RADIO_ON);
			}
			else
				SET_BIT(system_status, RADIO_ON);
			
		break;
		case FORMAT_TOGGLE:
			TOGGLE_BIT(Time.TimeFormat, TFORMAT_BIT);
			TOGGLE_BIT(AlarmTime.TimeFormat, TFORMAT_BIT);
			TOGGLE_BIT(system_status, TIME_FORMAT);
		break;
		default:
			//Display(Time.TimeFormat);
			//DisplayEncoder();
			volumeAdj();
			stationAdj();
			write2Bar(Time.sec);
			DisplayTime(&Time, ColonControl);
			//testPrint(extTempArr[0]);
			twi_start_rd(0x90, data, 2);
			temp = ((data[0] << 8)|(data[1]))/128;
			exttemp = ((extTempArr[1] << 8)|(extTempArr[0]))/128;
			itoa(temp, tempin, 10);
			itoa(exttemp, tempout, 10);
			LCD_IPainter(tempin, tempout, system_status, current_fm_freq);
		break;
	}
}

void SoundAlarm(){
	setVolume(0x03FF);
	AlarmDisplayTime(&Time, ColonControl);
	setFrequency(C5);
	_delay_ms(10);
}

void SetupInterrupt(){
	EICRB|=(1<<ISC70)|(1<<ISC71); //enable external interupt 7 rising edge
	EIMSK|=(1<<INT7);
}

int main(){
	//Initializations
	Timer0Setup();
	timer1_init();
	Timer2Setup();
	init_DeviceDependencies();
	Timer3Setup();
	init_SPI();
	ADC0Setup();
	DDRD |= 0x03;
	init_twi();
	uart_init();
	setVolume(current_volume);
	uint8_t datainit[2] = {0x00, 0x00};
	
	twi_start_wr(0x90, datainit, 2);
	SetupInterrupt();
	sei();   //Global Interrupt Enable
	
	write2Bar(CurMode);
	
	LCD_Init();
	LCD_MovCursorLn1();
	
	//init_radio();
	
	while(1){
		getMode();
		//if (AlarmArmed == 1)
			//if (AlarmTime.hour == Time.hour && AlarmTime.min == Time.min)
			//{
				//SoundAlarm();
			//}
		//}
	}
}