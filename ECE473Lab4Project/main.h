/*
 * main.h
 *
 * Created: 11/6/2016 12:25:16 PM
 *  Author: gbone
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define PinToggle(PORTX, pin) PORTX ^= (1<<pin)
#define PinHIGH(PORTX, pin) PORTX |= (1<<pin)
#define PinLOW(PORTX, pin) PORTX &= ~(1<<pin)
#define pinInput(DDRX, pin) DDRX &= ~(1<<pin)
#define pinOutput(DDRX, pin) DDRX |= (1<<pin)
#define port2Input(DDRX) DDRX = 0x00;
#define port2Output(DDRX) DDRX = 0xFF;

#define PWMPin PB7
#define PWMPort PORTB
#define PWMDDR DDRB

#define CHECK_BIT(byte, bit)	(byte & (1<<bit))
#define SET_BIT(byte, bit)		(byte |= (1 << bit))
#define CLEAR_BIT(byte, bit)	(byte &= ~(1 << bit))
#define TOGGLE_BIT(byte, bit)	(byte ^= (1 << bit))

#define ALARM_ARMED 0x01
#define TIME_FORMAT 0x02
#define ALARM_SOURCE 0x04

#define CLOCK_SET 0x01
#define ALARM_SET 0x02
#define RADIO_MODE 0x04
#define RADIO_TOGGLE 0x80
#define FORMAT_TOGGLE 0x40

#define RADIO_ON 0x02
#define ALARM_ON 0x01

#define TFORMAT_BIT 0x00
#define PMFRMT 0x01

uint8_t readButtons();

#endif /* MAIN_H_ */