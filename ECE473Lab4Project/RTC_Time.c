/*
 * RTC_Time.c
 *
 * Created: 11/4/2016 7:20:57 PM
 *  Author: gbone
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "RTC_Time.h"
#include <stdlib.h>

void incrementTime(RTC_Time* Current_Time){
	Current_Time->sec++;
	if(Current_Time->sec >= 60)
	{
		Current_Time->min++;
		Current_Time->sec = 0;
	}
	
	if (Current_Time->min >= 60)
	{
		Current_Time->hour++;
		Current_Time->min = 0;
	}
	
	if (Current_Time->hour >= 24)
	{
		Current_Time->hour = 0;
		Current_Time->min = 0;
		Current_Time->sec = 0;
	}
}


