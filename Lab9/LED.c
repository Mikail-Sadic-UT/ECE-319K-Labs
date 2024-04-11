/*
 * LED.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

#define R3D (1<<17)
#define YEL (1<<28) //Info
#define GRN (1<<31)

// initialize your LEDs     PA17 PA28 PA31
void LED_Init(void){
    IOMUX->SECCFG.PINCM[PA17INDEX]  = 0x0000081;
    IOMUX->SECCFG.PINCM[PA28INDEX]  = 0x0000081;
    IOMUX->SECCFG.PINCM[PA31INDEX]  = 0x0000081;
    GPIOA->DOESET31_0 = R3D | YEL | GRN;
}
// data specifies which LED to turn on
void LED_On(uint32_t data){
    GPIOA->DOUTSET31_0 = data;
}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    GPIOA->DOUTCLR31_0 = data;
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    GPIOA->DOUTTGL31_0 = data;
}
