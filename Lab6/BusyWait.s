// BusyWait.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


// Note: these functions do not actually output to SPI or Port A. 
// They are called by the grader to see if the functions would work

// As part of Lab 6, students need to implement these two functions
      .global   SPIOutCommand
      .global   SPIOutData
      .text
      .align 2

// ***********SPIOutCommand*****************
// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: R0 = 32-bit command (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI tx data register address
//         R3 = 32-bit GPIOA->DOUTCLR31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the clear register to clear will make it friendly

SPIOutCommand:
// --UUU-- Code to write a command to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
//3) Clear D/C (GPIO PA13) to zero, be friendly (R3 has address of GPIOA->DOUTCLR31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTCLR31_0
//4) Write the command to the SPI data register (R2 has address of SPI1->TXDATA)
//5) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)

    PUSH {R4-R7, LR}
    LDR R6, =(1<<4) //Mask for bit4
Loop1:
	//Busy-Wait
	LDR R7, [R1]	//Read SPI Status Reg
	ANDS R7, R7, R6	//Mask bit4
    CMP R7, #16		//Check bit4
    BEQ Loop1		//If high loop
    //Clear D/C, Write Command
    LDR R5, =0x2000	//Clear mask
    STR R5, [R3]	//Clear GPIO PA13
    STR R0, [R2]	//Write Command to SPI Data Reg
	//Busy-Wait
Loop2:
	LDR R7, [R1]	//Read SPI Status Reg
    ANDS R7, R7, R6	//Mask bit4
    CMP R7, #16		//Check bit4
    BEQ Loop2

    POP {R4-R7, PC}    //   return



// ***********SPIOutData*****************
// This is a helper function that sends an 8-bit data to the LCD.
// Inputs: R0 = 32-bit data (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI data register address
//         R3 = 32-bit GPIOA->DOUTSET31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the set register to clear will make it friendly

SPIOutData:
// --UUU-- Code to write data to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 1,
//2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
//3) Set D/C (GPIO PA13) to one, be friendly (R3 has address of GPIOA->DOUTSET31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTSET31_0
//4) Write the data to the SPI data register (R2 has address of SPI1->TXDATA)
    PUSH {R4-R7, LR}
	LDR R7, =2 //Mask for bit1
Loop3:
	LDR R5, [R1]	//Read SPI Stat Reg
	ANDS R5, R5, R7	//Mask bit1
	CMP R5, #0		//Check if low
	BEQ Loop3		//Loop if low

	LDR R6, =0x2000	//Set mask
	STR R6, [R3]	//Set GPIO PA13

	STR R0, [R2]	//Write Data to SPI Data Reg

    POP {R4-R7, PC}   // return
//****************************************************

    .end
