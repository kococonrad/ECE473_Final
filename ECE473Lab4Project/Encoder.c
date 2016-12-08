/*
 * Encoder.c
 *
 * Created: 11/4/2016 8:53:44 PM
 *  Author: gbone
 */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include<stdint.h>
#include<avr/io.h>
#include<util/delay.h>
#include "Encoder.h"
#include "main.h"

uint8_t Enc1_cur = 0;
uint8_t Enc1_past = 0;
uint8_t Enc2_cur = 0;
uint8_t Enc2_past = 0;

//Input: NULL
//Output: NULL
//	Allows the shifting in of current encoder values via ENC_LoadPin
void shiftInEncoder(){
	PinLOW(ENC_PORT, ENC_LoadPin);
	_delay_us(5);
	PinHIGH(ENC_PORT, ENC_LoadPin);
}

//Input: NULL
//Output: NULL
//	Reads in current values of both decoders to the Enc1_past and Enc2_past values
uint8_t getCurrentEncoderStates(){
	uint8_t val;
	shiftInEncoder();
	
	PinLOW(ENC_PORT, ENC_EnPin);
	SPDR = 0x00;
	while(bit_is_clear(SPSR, SPIF));
	PinHIGH(ENC_PORT, ENC_EnPin);
	val = SPDR;
	
	return val;
}

//Input: NULL
//Output: Debounces the encoders and returns the direction being turned
uint8_t readEncoders(){
	uint8_t currentEncoderState = getCurrentEncoderStates();

	//Get encoder 1 state
	if(~(Enc1_past == 0xFF) ||
	~(currentEncoderState & ENC1MASK))
	Enc1_past = (Enc1_past<<1) | ((currentEncoderState & ENC1MASK) ? 1 : 0);

	//Get encoder 2 state
	if(~(Enc2_past == 0xFF) ||
	~(currentEncoderState & ENC2MASK))
	Enc2_past = (Enc2_past<<1) | ((currentEncoderState & ENC2MASK) ? 1 : 0);

	//Translate values to direction ENCODER1
	if((Enc1_past & 0x3F) == 0x3E){
		if(currentEncoderState & 0x04) return REV1;
		else return FWD1;
	}
	//Tranlsates values to direction Encoder2
	else if((Enc2_past & 0x3F) == 0x3E){
		if(currentEncoderState & 0x01) return REV2;
		else return FWD2;
	}
	return REST;
}

uint8_t readEncoder1(){
	uint8_t currentEncoderState = getCurrentEncoderStates();

	//Get encoder 1 state
	if(~(Enc1_past == 0xFF) || ~(currentEncoderState & ENC1MASK))
		Enc1_past = (Enc1_past<<1) | ((currentEncoderState & ENC1MASK) ? 1 : 0);
	
	//Translate values to direction ENCODER1
	if((Enc1_past & 0x3F) == 0x3E){
		if(currentEncoderState & 0x04) return REV1;
		else return FWD1;
	}
	return REST;
}

uint8_t readEncoder2(){
	uint8_t currentEncoderState = getCurrentEncoderStates();
	
	//Get encoder 2 state
	if(~(Enc2_past == 0xFF) || ~(currentEncoderState & ENC2MASK))
		Enc2_past = (Enc2_past<<1) | ((currentEncoderState & ENC2MASK) ? 1 : 0);
	
	//Tranlsates values to direction Encoder2
	if((Enc2_past & 0x3F) == 0x3E){
		if(currentEncoderState & 0x01) return REV1;
		else return FWD1;
	}
	return REST;
}
