// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Jonathan Valvano
// 11/15/2021 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

uint32_t x; //index

const uint8_t   octave[98] = {14,8,10,8,11,14,9,9,9,11,18,18,15,16,16,16,16,19,23,22,22,22,22,16,8,9,9,9,9,10,15,16,15,17,14,12,11,9,9,10,10,9,13,17,16,16,16,17,10,2,4,3,4,4,4,11,17,18,17,18,25,26,23,23,23,23,23,26,30,29,29,29,29,23,15,16,16,15,16,16,21,23,21,23,20,18,17,15,15,16,15,15,18,22,22,21,22,22};

void SysTick_IntArm(uint32_t period, uint32_t priority){
    x = 0;
    SysTick->CTRL = 0;
    SysTick->LOAD = (period - 1);
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000)) | priority<<30;   //double check priority
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
    SysTick_IntArm(1, 1);
}
void SysTick_Handler(void){ // called at 11 kHz // output one value to DAC if a sound is active
	DAC5_Out(octave[x]);
	x++;
}

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
void Sound_Start(const uint8_t *pt, uint32_t count){
    SysTick->LOAD = (count - 1);
    SysTick->VAL = 0;
}

void Sound_Warp(void){
// write this
  
}
void Sound_Crash(void){
// write this
  
}
void Sound_Explosion(void){
// write this
 
}



