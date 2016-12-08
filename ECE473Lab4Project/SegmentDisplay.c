/*
 * SegmentDisplay.c
 *
 * Created: 11/4/2016 7:58:56 PM
 *  Author: gbone
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include<avr/io.h>
#include "SegmentDisplay.h"
#include "WaveGeneration.h"
#include "RTC_Time.h"
#include "Encoder.h"
#include "main.h"

volatile SegmentDigits digitValues;

void AlarmDisplayTime(RTC_Time* CurrentTime, uint8_t flashColon){
	uint8_t DigitDelay = 100;
	int count = 0;
	DDRA = 0xFF;
	set7SegmentDigits_Time(CurrentTime);
	
	DigitSelectPort = Digit4;
	DigitsPort = ~(digitValues.digit4);
	_delay_ms(DigitDelay);
	DigitsPort = 0xFF;
	_delay_ms(DigitDelay);
	
	DigitSelectPort = Digit3;
	DigitsPort = ~(digitValues.digit3);
	_delay_ms(DigitDelay);
	DigitsPort = 0xFF;
	_delay_ms(DigitDelay);
	
	DigitSelectPort = Colon;
	DigitsPort = ~(A|B|~C);
	_delay_ms(DigitDelay);
	DigitsPort = 0xFF;
	_delay_ms(DigitDelay);
	
	DigitSelectPort = Digit2;
	DigitsPort = ~(digitValues.digit2);
	_delay_ms(DigitDelay);
	DigitsPort = 0xFF;
	_delay_ms(DigitDelay);
	
	DigitSelectPort = Digit1;
	DigitsPort = ~(digitValues.digit1);
	_delay_ms(DigitDelay);
	DigitsPort = 0xFF;
	_delay_ms(DigitDelay);
	
	
	do 
	{
		setFrequency(1200);
		DisplayTime(CurrentTime, 1);
		count++;
	} while (count< 300);
	setFrequency(0);
}

void DisplayHour(RTC_Time* CurrentTime){
	uint8_t zeroState = 0x00;
	DDRA = 0xFF;
	set7SegmentDigits_Time(CurrentTime);
	
	DigitSelectPort = Digit4;
	if (digitValues.digit4 == Zero)
	{
		zeroState |= Digit4ZeroBit;
		DigitsPort = 0xFF;
	}
	else
	DigitsPort = ~(digitValues.digit4);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit3;
	DigitsPort = ~(digitValues.digit3);
	_delay_us(GhostingAdj);
	
	DigitSelectPort = Colon;
	DigitsPort = ~(A|B|~C);
	
	_delay_us(GhostingAdj);
	DigitsPort = 0xFF;
}

void DisplayMin(RTC_Time* CurrentTime){
	DDRA = 0xFF;
	set7SegmentDigits_Time(CurrentTime);
	
	DigitSelectPort = Digit2;
	DigitsPort = ~(digitValues.digit2);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit1;
	DigitsPort = ~(digitValues.digit1);
	_delay_us(GhostingAdj);
	
	DigitSelectPort = Colon;
	DigitsPort = ~(A|B|~C);
	
	_delay_us(GhostingAdj);
	DigitsPort = 0xFF;
}

void DisplayTime(RTC_Time* CurrentTime, uint8_t flashColon){
	uint8_t zeroState = 0x00;
	DDRA = 0xFF;
	set7SegmentDigits_Time(CurrentTime);
	
	DigitSelectPort = Digit4;
	if (digitValues.digit4 == Zero)
	{
		zeroState |= Digit4ZeroBit;
		DigitsPort = 0xFF;
	}
	else
		DigitsPort = ~(digitValues.digit4);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit3;
	DigitsPort = ~(digitValues.digit3);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit2;
	DigitsPort = ~(digitValues.digit2);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit1;
	DigitsPort = ~(digitValues.digit1);
	_delay_us(GhostingAdj);
	//Sets port A to output
	if (flashColon == 1)
	{
		DigitSelectPort = Colon;
		DigitsPort = ~(A|B|~C);
	}
	else
	{
		DigitSelectPort = Colon;
		DigitsPort = (A|B|C);
	}
	_delay_us(GhostingAdj);
	DigitsPort = 0xFF;
}

void Display(int Number){
	uint8_t zeroState = 0x00;
	DDRA = 0xFF;
	set7SegmentDigits_Number(Number);
	
	DigitSelectPort = Digit4;
	if (digitValues.digit4 == Zero)
	{
		zeroState |= Digit4ZeroBit;
		DigitsPort = 0xFF;
	}
	else
	DigitsPort = ~(digitValues.digit4);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit3;
	if (digitValues.digit3 == Zero && zeroState == Digit4ZeroBit)
	{
		zeroState |= Digit3ZeroBit;
		DigitsPort = 0xFF;
	}
	else
	DigitsPort = ~(digitValues.digit3);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit2;
	if (digitValues.digit2 == Zero && zeroState == D4_D3Zero)
	{
		zeroState |= Digit2ZeroBit;
		DigitsPort = 0xFF;
	}
	else
	DigitsPort = ~(digitValues.digit2);
	_delay_us(GhostingAdj);
	DigitSelectPort = Digit1;
	DigitsPort = ~(digitValues.digit1);
	_delay_us(GhostingAdj);
	//Sets port A to output
	DigitSelectPort = Colon;
	DigitsPort = (A|B|C);
	_delay_us(GhostingAdj);
	DigitsPort = 0xFF;
}

void setHour(RTC_Time* CurrentTime){
	uint8_t but_ret, enc_ret;
	do
	{
		enc_ret = readEncoders();
		if (enc_ret == FWD2)
		{
			CurrentTime->hour++;
			if (CurrentTime->hour > 23)
				CurrentTime->hour = 0;
			else if (CurrentTime->hour < 0)
				CurrentTime->hour = 23;
		}
		else if (REV2)
		{
			CurrentTime->hour--;
			if (CurrentTime->hour > 23)
				CurrentTime->hour = 0;
			else if (CurrentTime->hour < 0)
				CurrentTime->hour = 23;
		}
		
		but_ret = readButtons();
		
		DisplayHour(CurrentTime);
	} while (but_ret != 0x01);
}

void setMin(RTC_Time* CurrentTime){
	uint8_t but_ret, enc_ret;
	do
	{
		enc_ret = readEncoders();
		if (enc_ret == FWD1)
		{
			CurrentTime->min++;
			if (CurrentTime->min > 59)
			CurrentTime->min = 0;
			else if (CurrentTime->min < 0)
			CurrentTime->min = 59;
		}
		else if (enc_ret == REV1)
		{
			CurrentTime->min--;
			if (CurrentTime->min > 59)
			CurrentTime->min = 0;
			else if (CurrentTime->min < 0)
			CurrentTime->min = 59;
		}
		
		but_ret = readButtons();
		
		DisplayMin(CurrentTime);
	} while (but_ret != 0x01);
	CurrentTime->sec = 0;
}

uint8_t dec2Segments(uint8_t Number){
	switch (Number)
	{
		case 0:
		return Zero;
		break;
		case 1:
		return One;
		break;
		case 2:
		return Two;
		break;
		case 3:
		return Three;
		break;
		case 4:
		return Four;
		break;
		case 5:
		return Five;
		break;
		case 6:
		return Six;
		break;
		case 7:
		return Seven;
		break;
		case 8:
		return Eight;
		break;
		case 9:
		return Nine;
		break;
		default:
		return 0xFF;
		break;
	}
}

void set7SegmentDigits_Time(RTC_Time* CurrentTime){
	if (CHECK_BIT(CurrentTime->TimeFormat, TFORMAT_BIT))
	{
		if (CurrentTime->hour > 12)
		{
			CurrentTime->hour -= 12;
			//SET_BIT(CurrentTime->TimeFormat |= (1 << PMFRMT);
			digitValues.digit3 = (dec2Segments(CurrentTime->hour%10))|DEC;
		}
		else if (CurrentTime->hour == 0)
		{
			CurrentTime->hour += 12;
			//CurrentTime->TimeFormat |= ~(1 << PMFRMT);
			digitValues.digit3 = (dec2Segments(CurrentTime->hour%10));
		}
		else
		{
			//CurrentTime->TimeFormat |= ~(1 << PMFRMT);
			digitValues.digit3 = dec2Segments(CurrentTime->hour%10);
		}
		
		digitValues.digit4 = dec2Segments((CurrentTime->hour/10)%10);
		digitValues.digit2 = dec2Segments((CurrentTime->min/10)%10);
		digitValues.digit1 = (dec2Segments(CurrentTime->min%10));
	}
	else
	{
		digitValues.digit4 = dec2Segments((CurrentTime->hour/10)%10);
		digitValues.digit3 = dec2Segments(CurrentTime->hour%10);
		digitValues.digit2 = dec2Segments((CurrentTime->min/10)%10);
		digitValues.digit1 = dec2Segments(CurrentTime->min%10);
	}
}

void set7SegmentDigits_Number(short Number){
	digitValues.digit4 = dec2Segments(Number/1000);
	digitValues.digit3 = dec2Segments((Number/100)%10);
	digitValues.digit2 = dec2Segments((Number/10)%10);
	digitValues.digit1 = dec2Segments(Number%10);
}

void Timer2Setup(){
	TCCR2 = (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(1<<COM20)|(1<<CS20);
	
	PWMDDR |= (1<<PWMPin);
}

void setBrightness(uint8_t duty){
	if (duty <= 1)
	{
		OCR2 = 2;
	}
	else
	{
		OCR2 = duty;
	}
}

void ADC0Setup(){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1<< ADEN) | (1 << ADFR) | (1 << ADSC) | (1 << ADIE);
	ADMUX |= (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);
}