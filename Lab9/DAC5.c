/* DAC5.c
 * Students put your names here
 * Modified: January 1, 2024
 * 5-bit binary-weighted DAC connected to PB4-PB0
 */
#include <ti/devices/msp/msp.h>
#define PB0INDEX  11 // UART0_TX  SPI1_CS2  TIMA1_C0  TIMA0_C2
#define PB1INDEX  12 // UART0_RX  SPI1_CS3  TIMA1_C1  TIMA0_C2N
#define PB2INDEX  14 // UART3_TX  UART2_CTS I2C1_SCL  TIMA0_C3  UART1_CTS TIMG6_C0  TIMA1_C0
#define PB3INDEX  15 // UART3_RX  UART2_RTS I2C1_SDA  TIMA0_C3N UART1_RTS TIMG6_C1  TIMA1_C1
#define PB4INDEX  16 // UART1_TX  UART3_CTS TIMA1_C0  TIMA0_C2  TIMA1_C0N

// **************DAC5_Init*********************
// Initialize 5-bit DAC, called once
// Input: none
// Output: none
void DAC5_Init(void){
// Assumes LaunchPad_Init has been called
// I.e., PortB has already been reset and activated (do not reset PortB here again)
  // write this

        //DAC outputs
        IOMUX->SECCFG.PINCM[PB0INDEX] = 0x0000081;
        IOMUX->SECCFG.PINCM[PB1INDEX] = 0x0000081;
        IOMUX->SECCFG.PINCM[PB2INDEX] = 0x0000081;
        IOMUX->SECCFG.PINCM[PB3INDEX] = 0x0000081;
        IOMUX->SECCFG.PINCM[PB4INDEX] = 0x0000081;
        //IOMUX->SECCFG.PINCM[PB6INDEX] = 0x0000081;

        //Enable
        GPIOB->DOESET31_0 = 1 | (1<<1) | (1<<2) | (1<<3) | (1<<4);
}

// **************DAC5_Out*********************
// output to DAC5
// Input: data is 5-bit integer, 0 to 31
// Output: none
// Note: this solution must be friendly
uint32_t prevData = 0;
void DAC5_Out(uint32_t data){   // write this
    GPIOB->DOUTCLR31_0 = prevData;  //Frendly :)
    GPIOB->DOUTSET31_0 = data;
    prevData = data;
}
