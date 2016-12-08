/*
 * Encoder.h
 *
 * Created: 11/4/2016 8:53:53 PM
 *  Author: gbone
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_
#include <stdint.h>

//Encoder definitions
//states for REST FWD and REV
#define REST 0
#define FWD1 0x01
#define REV1 0x02
#define FWD2 0x04
#define REV2 0x08

//Encoder values to read current/past states and masks to read individual values
#define ENC1MASK 0x08
#define ENC2MASK 0x02
#define Enc1_Mask 0x03;
#define Enc2_Mask 0x0C;
#define Enc2_Shift 0x02;
#define ENC_LoadPin PE4
#define ENC_EnPin PE6
#define ENC_DDR DDRE
#define ENC_PORT PORTE

//Input: NULL
//Output: NULL
//	Allows the shifting in of current encoder values via ENC_LoadPin
void shiftInEncoder();
//Input: NULL
//Output: NULL
//	Reads in current values of both decoders to the Enc1_past and Enc2_past values
uint8_t getCurrentEncoderStates();
//Input: NULL
//Output: Debounces the encoders and returns the direction being turned
uint8_t readEncoders();
uint8_t readEncoder1();
uint8_t readEncoder2();
#endif /* ENCODER_H_ */