/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

#define UP (1<<24)
#define DWN (1<<26)
#define LFT (1<<25)
#define RT (1<<27)

#define SP  (1<<16)

// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){ //PA24, 25, 26, 27, PB16
    IOMUX->SECCFG.PINCM[PA24INDEX] = (uint32_t) 0x00050081; // input, pull down
    IOMUX->SECCFG.PINCM[PA25INDEX] = (uint32_t) 0x00050081; // input, pull down
    IOMUX->SECCFG.PINCM[PA26INDEX] = (uint32_t) 0x00050081; // input, pull down
    IOMUX->SECCFG.PINCM[PA27INDEX] = (uint32_t) 0x00050081; // input, pull down
    IOMUX->SECCFG.PINCM[PB16INDEX] = (uint32_t) 0x00050081; // input, pull down
}
// return current state of switches
uint32_t Switch_InA(void){
    return (GPIOA->DIN31_0 & (UP + DWN + LFT + RT));
}

uint32_t Switch_InB(void){
    return (GPIOB->DIN31_0 & SP);
}
