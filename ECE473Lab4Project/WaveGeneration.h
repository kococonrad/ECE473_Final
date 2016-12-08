/*
 * WaveGeneration.h
 *
 * Created: 11/6/2016 11:46:36 PM
 *  Author: gbone
 */ 


#ifndef WAVEGENERATION_H_
#define WAVEGENERATION_H_

 #define C5 523
 #define CSharp5 554
 #define D5 587
 #define DSharp5 622
 #define E5 659
 #define F5 698
 #define FSharp5 740
 #define G5 784
 #define GSharp5 831
 #define A5 880
 #define ASharp5 932
 #define B5 988

 void timer1_init();
 /// Set to the given frequency
 void setFrequency(int frequency);

void Timer3Setup();
void setVolume(uint16_t duty);
 void volumeAdj();
 void MuteAll();
#endif /* WAVEGENERATION_H_ */