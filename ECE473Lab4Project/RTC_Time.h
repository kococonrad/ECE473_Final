/*
 * RTC_Time.h
 *
 * Created: 11/4/2016 7:19:28 PM
 *  Author: gbone
 */ 
#ifndef RTC_TIME_H_
#define RTC_TIME_H_
#include <stdint.h>

#define SETBUTTON 0x80

typedef struct {
	short sec;
	short min;
	short hour;
	uint8_t TimeFormat;
} RTC_Time;

void incrementTime(RTC_Time* Current_Time);
void writeTime(const RTC_Time* Current_Time);

#endif /* RTC_TIME_H_ */