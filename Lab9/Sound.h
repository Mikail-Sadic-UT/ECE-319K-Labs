// Sound.h
// Runs on MSPM0
// Play sounds on 5-bit DAC.
// Your name
// 11/5/2023
#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
// This is called once
void Sound_Init(void);

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(uint32_t count, uint8_t mode);

void Sound_Stop();

// following 8 functions do not output to the DAC
// they configure pointers/counters and initiate the sound by calling Sound_Start
void Octave();
void Sound_Crash(void);
void Sound_Explosion(void);
void Sound_Warp(void);
void Sound_Hit(void);


#endif
