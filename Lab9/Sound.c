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

uint16_t x; //index

void SysTick_IntArm(uint32_t period, uint32_t priority){
    x = 0;
    SysTick->CTRL = 0;
    SysTick->LOAD = (period - 1);
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000)) | priority<<30; //Copied this from the textbook so I think it's probably right
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
    SysTick_IntArm(1, 1);
    DAC5_Init();
}

uint8_t MODE;

void SysTick_Handler(void){ // called at 11 kHz // output one value to DAC if a sound is active
	if(MODE == 1) Sound_Hit();
	if(MODE == 2) Sound_Crash();
	if(MODE == 3) Sound_Warp();
	if(MODE == 4) Sound_Explosion();
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
void Sound_Start(uint32_t count, uint8_t mode){
    SysTick->LOAD = (count - 1);
    SysTick->VAL = 0;
    MODE = mode;
    x = 0;
}

void Sound_Stop(){
    SysTick->LOAD = 0;
    x = 0;
}

void Sound_Warp(void){
    if(x > 4205) Sound_Stop();
    DAC5_Out(Warp[x]);
    x++;
}

void Sound_Hit(void){
    if(x > 375) Sound_Stop();
    DAC5_Out(Hit[x]);
    x++;
}

void Sound_Crash(void){
    if(x > 3082) Sound_Stop();
    DAC5_Out(Crash[x]);
    x++;
}

void Sound_Explosion(void){
    if(x > 8731) Sound_Stop();
    DAC5_Out(explosion[x]);
    x++;
}

void Octave(){ //debug
    if(x > 98) x = 0;
    DAC5_Out(octave[x]);
    x++;
}

