/* ECE319K_Lab5main.c
 * Digital Piano using a Digital to Analog Converter
 * December 28, 2023
 * 5-bit binary-weighted DAC connected to PB4-PB0
 * 4-bit keyboard connected to PB19-PB16
 * Key0=311.1, Key1=392.0, Key2=466.2, Key3=523.3 Hz
*/

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include "../inc/DAC5.h"  // student's Lab 5
#include "../inc/Key.h"   // student's Lab 5
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "MFS2447"; // replace abc123 with your EID
const char EID2[] = "JK49263"; // replace abc123 with your EID

// prototypes to your low-level Lab 5 code
void Sound_Init(uint32_t period, uint32_t priority);
void Sound_Start(uint32_t period);
void Sound_Stop(void);

// use main1 to determine Lab5 assignment
void Lab5Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(0); // print assignment, no grading
  while(1){
  }
}
const uint32_t Inputs[12]={0, 1, 7, 8, 15, 16, 17, 23, 24, 25, 30, 31};
uint32_t Testdata;

// use main2a to perform static testing of DAC, if you have a voltmeter
int main2a(void){ // main2a
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      // put a breakpoint on the next line and use meter to measure DACout
      // place data in Table 5.3
      Debug_Dump(Testdata);
    }
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main2b to perform static testing of DAC, if you do not have a voltmeter
// attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To use the scope, there can be no breakpoints in your code
int main2b(void){ // main2b
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    while(1){}; // access to GPIOB->DOE31_0 should be friendly
  }
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<12; i++){ //0-11
      Testdata = Inputs[i];
      DAC5_Out(Testdata);
      Debug_Dump(Testdata);
        // use TExaSdisplay scope to measure DACout
        // place data in Table 5.3
        // touch and release S2 to continue
      //Clock_Delay(80000);
      while(LaunchPad_InS2()==0){}; // wait for S2 to be touched
      while(LaunchPad_InS2()!=0){}; // wait for S2 to be released
      if((GPIOB->DOUT31_0&0x20) == 0){
         while(1){}; // DOUT not friendly
      }
    }
  }
}
// use main3 to perform dynamic testing of DAC,
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a monotonic ramp with period 32ms,

const uint8_t SinWave[32] = {16,19,22,24,27,28,30,31,31,31,30,
                             28,27,24,22,19,16,13,10,8,5,4,
                             2,1,1,1,2,4,5,8,10,13};    //I copy pasted this from wbutton3 they gave us, looks right to me (i hope)

int main3(void){ // main3
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // Scope
  DAC5_Init();     // your Lab 5 initialization
  Debug_Init();    // Lab 3 debugging
  while(1){
    for(uint32_t i=0; i<32; i++){ //0-31
      DAC5_Out(i);
      Debug_Dump(i);
        // scope to observe waveform
        // place data in Table 5.3
      Clock_Delay1ms(1);
    }
  }
}

// use main4 to debug the four input switches
int main4(void){ // main4
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Key_Init(); // your Lab 5 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 5, Spring 2024, Step 4. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Key_In(); // Your Lab5 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}

// use main5 to debug your system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Sound_Start(9956); // start one continuous wave
  while(1){
  }
}


// use main6 to debug/grade your final system
// In lab, attach your DACout to the real scope
// If you do not have a scope attach PB20 (scope uses PB20 as ADC input) to your DACout
// TExaSdisplay scope uses TimerG7, ADC0
// To perform dynamic testing, there can be no breakpoints in your code
// DACout will be a sine wave with period/frequency depending on which key is pressed

//Key0=311.1, Key1=392.0, Key2=466.2, Key3=523.3 Hz
#define EF0 8035   // 311.1 Hz
#define G0  6378   // 392 Hz
#define BF0 5363   // 466.2 Hz
#define C0  4778   // 523.3 Hz

const uint8_t SquareWave[32] = {31,31,31,31,31,31,31,31,31,31,31,
                             31,31,31,31,31,1,1,1,1,1,1,
                             1,1,1,1,1,1,1,1,1,1};

const uint8_t Trumpet[64] = {
  20,21,21,22,23,23,23,23,21,18,15,
  10,6,3,2,6,15,23,28,29,29,26,
  23,22,21,21,20,21,21,21,21,20,
  20,20,20,20,20,21,21,20,20,20,
  20,20,21,21,22,22,21,21,21,21,
  21,21,22,22,21,21,21,20,20,20,20,20
};


int main6a(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(3);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging

  uint32_t preVal = 0;
  uint32_t curVal;
  while(1){
// if key goes from not pressed to pressed
//   -call Sound_Start with the appropriate period
//   -call Debug_Dump with period
// if key goes from pressed to not pressed
//   -call Sound_Stop
// I.e., if key has not changed DO NOT CALL start or stop
    curVal = Key_In();
    if(curVal != preVal){
        if(curVal == 0x1){
            Sound_Start(EF0);
        }
        if(curVal == 0x2){
            Sound_Start(G0);
        }
        if(curVal == 0x4){
            Sound_Start(BF0);
        }
        if(curVal == 0x8){
            Sound_Start(C0);
        }
    }
    if(curVal == 0){
        Sound_Stop();
    }
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// To grade you must connect PB20 to your DACout
// Run main6 with Lab5Grader(3);   // Grader
// Observe Terminal window

// Lab 5 low-level function implementations
// Key0=311.1, Key1=392.0, Key2=466.2, Key3=523.3 Hz


// ARM SysTick period interrupts
// Input: interrupts every 12.5ns*period
//        priority is 0 (highest) to 3 (lowest)
#define button1 1
#define button2 1<<1
#define button3 1<<2
#define button4 1<<3

#define shootp 4080
#define squarep 1885
#define A0  5682   // 440 Hz
#define A0t  2841   // 440 Hz

#define EF02 2624   // 311.1 Hz
#define G02  2082   // 392 Hz
#define BF02 1751   // 466.2 Hz
#define C02  1560   // 523.3 Hz


uint8_t handlerMode = 0;
int main6b(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging

  uint32_t preVal = 0;
  uint32_t curVal;
  uint32_t played = 0;
  while(1){
// if key goes from not pressed to pressed
//   -call Sound_Start with the appropriate period
//   -call Debug_Dump with period
// if key goes from pressed to not pressed
//   -call Sound_Stop
// I.e., if key has not changed DO NOT CALL start or stop
    curVal = Key_In();
    if((curVal != preVal)){
        if(curVal == button1){
            handlerMode = button1;
            Sound_Start(EF02);
        }
        if(curVal == button2){
            handlerMode = button2;
            Sound_Start(G02);
        }
        if(curVal == button3){
            handlerMode = button3;
            Sound_Start(BF02);
        }
        if(curVal == button4){
            handlerMode = button4;
            Sound_Start(C02);
        }
    }
    if(curVal == 0){
        Sound_Stop();
    }
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}


#define A03  1855   // 440 Hz
#define E   1238   // 659.3 Hz
#define F   1169   // 698.5 Hz
#define G    1041   // 784 Hz
#define C03  1560   // 523.3 Hz
#define D   1390   // 587.3 Hz
#define A    928   // 880 Hz
#define C    780   // 1046.5 Hz


int main(void){// main6
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();   // execute this line before your code
  Lab5Grader(2);   // 1=logic analyzer, 2=Scope, 3=grade
  DAC5_Init();     // DAC initialization
  Sound_Init(1,0); // SysTick initialization, initially off, priority 0
  Key_Init();      // Keyboard initialization
  Debug_Init();    // Lab 3 debugging

  uint32_t preVal = 0;
  uint32_t curVal;
  uint32_t played = 0;
  while(1){
// if key goes from not pressed to pressed
//   -call Sound_Start with the appropriate period
//   -call Debug_Dump with period
// if key goes from pressed to not pressed
//   -call Sound_Stop
// I.e., if key has not changed DO NOT CALL start or stop
    curVal = Key_In();
    if((curVal != preVal)){
        if(curVal == 0x1){
            Sound_Start(A03);
        }
        if(curVal == 0x2){
            Sound_Start(E);
        }
        if(curVal ==0x4){
            Sound_Start(F);
        }
        if(curVal == 0x8){
            Sound_Start(G);
        }
        if(curVal == 0x3){
            Sound_Start(C03);
        }
        if(curVal == 0x6){
            Sound_Start(D);
        }
        if(curVal == 0xC){
            Sound_Start(A);
        }
        if(curVal == 0xE){
            Sound_Start(C);
        }
    }
    if(curVal == 0){
        Sound_Stop();
    }
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}

uint32_t x; //index

void Sound_Init(uint32_t period, uint32_t priority){    // write this
    x = 0;
    SysTick->CTRL = 0;
    SysTick->LOAD = (period - 1);
    SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000)) | priority<<30; //Copied this from the textbook so I think it's probably right
    SysTick->VAL = 0;
    SysTick->CTRL = 0x07;
}
void Sound_Stop(void){ // write this
    // either set LOAD to 0 or clear bit 1 in CTRL
    SysTick->LOAD = 0;
    GPIOB->DOUTCLR31_0 = 0x01F;
}   //I think this is done?


void Sound_Start(uint32_t period){ // write this
    // set reload value
    SysTick->LOAD = (period - 1);
    SysTick->VAL = 0;   // write any value to VAL, cause reload
}   //This needs to be double checked

const uint8_t   shoot[4080] = {8,6,6,10,12,8,2,6,12,7,3,5,9,13,9,3,5,9,10,7,4,8,12,8,3,5,8,12,10,4,4,9,11,8,4,7,12,8,4,6,9,10,6,3,7,11,8,4,6,13,13,6,2,5,9,7,2,4,9,12,11,12,11,7,5,2,-1,5,10,6,7,13,15,15,7,0,0,0,0,0,5,14,15,15,15,15,11,0,0,0,1,10,9,8,15,15,15,11,2,0,0,1,0,3,10,14,15,14,16,14,4,-1,1,2,3,3,7,13,14,15,15,9,3,0,0,0,1,8,12,12,14,14,16,11,1,-1,1,0,1,2,8,15,15,15,15,15,8,0,0,1,4,5,4,10,15,15,15,13,9,3,0,0,1,6,5,7,13,14,15,14,9,4,0,0,0,0,8,10,9,13,15,15,15,5,0,0,0,0,0,5,13,13,14,15,15,15,7,0,0,0,4,4,3,7,12,15,15,15,13,7,3,1,-1,1,6,8,6,11,15,15,15,13,8,4,1,-1,1,5,6,5,11,15,15,15,14,12,6,1,0,0,3,7,7,7,13,15,14,16,13,6,2,0,0,0,1,7,12,10,11,15,15,15,11,2,0,0,0,0,2,9,12,11,13,14,15,14,9,4,1,0,0,0,6,9,7,11,15,15,15,14,12,7,0,0,0,2,5,5,7,12,14,15,15,15,15,7,1,0,0,2,2,2,6,11,15,15,15,14,16,12,3,0,0,0,3,3,3,8,13,15,14,15,15,15,13,3,-1,1,0,1,2,5,10,13,13,13,15,15,14,9,2,0,0,0,0,1,6,11,11,10,14,15,14,15,12,3,0,0,0,0,0,4,9,13,12,11,14,15,15,14,8,2,0,0,0,0,1,7,12,10,9,13,15,15,15,12,8,4,0,0,0,0,5,7,7,9,11,14,15,14,14,14,13,5,0,0,0,0,1,3,8,12,12,11,14,15,14,14,12,8,4,0,0,0,0,5,6,6,11,13,12,13,15,15,15,12,6,3,0,0,0,0,5,6,7,10,12,14,13,13,15,15,13,9,4,1,0,0,0,2,6,7,8,12,13,12,13,15,15,14,11,8,5,1,0,0,0,3,5,6,9,12,14,13,12,14,15,15,13,8,5,2,0,0,0,2,6,7,8,10,12,14,13,12,14,15,15,12,6,2,0,0,0,0,2,7,8,8,10,11,13,14,13,12,14,15,13,8,3,0,0,0,0,0,4,9,10,10,10,10,12,15,13,12,13,14,12,7,3,1,0,0,0,0,4,9,10,9,10,12,13,12,11,13,14,14,11,8,6,3,0,0,0,1,3,5,7,10,12,12,10,9,11,14,13,12,12,12,12,7,1,0,0,0,0,1,5,10,10,10,10,11,13,11,10,12,13,14,12,10,9,7,4,0,0,0,0,1,3,7,9,12,13,10,9,10,12,11,10,11,13,12,10,8,5,3,0,0,0,0,3,5,6,9,11,12,12,10,10,11,11,11,11,12,13,10,9,7,6,3,0,0,0,0,4,5,6,9,12,11,11,10,10,11,10,9,11,11,12,12,9,9,8,6,2,0,0,0,0,1,4,7,10,12,11,10,11,11,10,8,9,11,12,11,10,11,11,9,8,6,4,2,0,0,0,1,5,6,7,9,11,12,11,10,10,11,10,8,9,11,12,11,10,10,10,10,8,5,3,2,1,0,0,1,4,7,8,9,10,10,11,11,10,9,9,10,9,8,9,11,11,10,9,10,10,8,6,5,4,1,0,1,2,3,3,5,8,10,11,10,10,11,10,9,9,9,9,8,8,9,10,11,10,9,9,9,9,8,6,6,4,1,0,1,1,3,4,5,6,9,10,11,11,10,10,10,10,9,8,8,9,8,8,8,9,10,10,9,9,9,9,9,7,6,5,3,3,2,1,1,2,4,5,6,7,9,10,11,10,10,10,10,10,8,8,9,9,8,7,8,8,9,9,9,10,10,9,8,8,8,8,7,6,5,4,3,2,2,3,3,4,4,5,7,9,9,10,10,11,11,10,9,9,9,9,8,8,8,8,8,7,8,8,8,8,8,9,10,9,9,8,8,8,8,7,7,7,7,5,4,3,3,3,3,3,4,5,6,6,7,8,9,10,10,10,10,10,10,9,9,9,9,8,8,8,8,8,8,8,8,8,8,7,8,8,8,8,8,9,9,9,8,8,8,7,7,7,6,7,7,7,5,3,1,0,1,3,6,8,9,12,15,15,10,5,3,3,6,13,12,5,4,4,7,14,12,5,4,4,7,14,12,5,4,4,7,14,12,5,4,4,7,14,12,5,4,4,6,14,12,4,3,3,6,14,13,5,4,3,6,14,13,5,3,3,5,13,14,6,3,3,5,12,15,7,3,3,4,11,15,8,3,3,4,10,15,10,3,3,3,8,15,11,3,3,3,7,15,13,4,3,3,6,14,14,6,3,3,5,12,15,8,3,3,4,9,15,10,3,3,3,7,15,12,4,3,3,6,14,14,6,3,3,4,11,15,9,3,3,3,8,15,12,4,3,3,6,14,14,5,3,3,4,11,15,9,3,3,3,8,15,12,4,2,3,5,13,15,7,3,3,4,9,15,11,3,3,3,6,15,14,5,2,3,4,10,15,10,3,3,3,7,15,14,5,2,3,4,10,15,10,3,3,3,6,15,14,5,3,3,4,10,15,10,3,3,3,6,14,15,6,2,3,4,9,15,12,4,2,3,5,12,15,8,2,3,3,7,15,14,5,2,3,4,9,15,11,3,2,3,5,13,15,8,2,3,3,7,15,14,5,2,3,4,9,15,12,4,2,3,5,11,15,10,3,3,4,6,13,15,7,2,3,3,7,15,14,6,2,3,4,8,15,13,4,2,3,4,10,15,11,4,2,3,5,11,15,10,3,2,4,5,12,15,9,3,3,4,6,13,15,8,3,3,4,7,13,15,8,2,3,4,7,13,15,7,2,3,4,7,14,15,7,3,3,4,7,14,15,7,2,3,4,7,14,15,8,3,3,4,7,13,15,8,3,3,4,6,12,15,9,3,3,4,6,11,15,10,3,3,4,5,10,15,11,4,2,3,4,9,15,12,4,2,3,4,8,15,13,5,2,3,4,7,14,15,7,2,3,4,6,12,15,9,3,2,4,5,10,15,11,4,2,4,5,8,15,14,6,2,3,4,7,12,15,9,3,3,4,6,10,15,12,4,2,4,5,8,14,15,6,2,3,4,6,11,15,10,3,3,3,5,9,15,14,6,3,3,4,7,11,15,10,3,3,4,5,9,15,14,6,3,3,4,7,11,15,11,4,3,3,5,8,14,15,7,3,3,4,6,10,15,12,4,3,3,5,7,12,15,9,3,3,3,6,8,14,15,6,3,3,4,6,9,15,12,4,3,3,5,7,10,15,11,4,3,3,5,7,12,15,9,3,3,4,6,8,13,15,7,3,3,4,6,8,14,14,6,3,3,4,6,9,15,14,5,3,3,4,6,9,15,13,5,3,3,5,6,9,15,13,5,3,3,4,6,9,15,13,5,3,3,4,6,9,15,13,5,3,3,4,6,9,14,14,6,3,3,4,6,8,14,15,7,3,3,4,6,8,12,15,9,3,3,4,5,7,11,15,10,4,3,3,5,7,10,15,12,4,3,3,4,6,9,14,14,6,3,3,4,6,7,12,15,9,3,3,4,6,7,10,15,12,4,3,3,5,7,8,13,15,8,3,3,4,6,7,11,15,11,4,3,3,5,7,8,14,14,7,3,3,4,6,7,10,15,11,4,3,4,5,7,8,13,15,9,3,3,4,5,7,9,15,13,6,3,4,4,6,7,10,15,11,4,3,4,5,7,8,12,15,10,4,3,3,5,7,8,13,15,8,3,3,4,6,7,9,14,14,7,3,3,4,6,7,9,14,13,6,3,3,4,6,7,9,14,13,6,3,3,4,6,7,9,14,13,6,3,3,4,6,7,9,14,13,6,3,3,4,6,7,9,14,14,8,3,3,4,6,7,8,13,15,9,4,3,4,5,7,8,11,15,11,4,3,4,5,7,7,10,15,13,6,3,3,4,6,7,9,13,14,8,3,3,4,5,7,8,11,15,11,5,3,4,5,7,7,9,14,14,8,3,3,4,5,7,8,11,15,11,5,3,4,4,6,7,9,13,15,9,4,3,4,5,7,7,10,14,13,7,3,3,4,6,7,8,11,15,12,5,3,3,4,6,7,8,12,15,11,4,3,4,5,6,7,8,12,15,10,4,3,3,5,6,7,8,12,15,10,4,3,3,5,7,7,9,12,15,10,4,3,3,5,6,7,8,11,15,11,4,3,3,5,6,7,8,10,15,12,5,3,4,4,6,7,8,9,13,14,8,3,3,4,6,7,7,8,12,15,10,4,3,4,5,6,7,8,10,14,13,6,3,3,4,6,7,8,9,12,15,10,4,3,4,5,6,7,8,10,14,13,7,3,3,4,6,7,8,8,10,14,12,5,3,3,4,6,7,8,9,11,15,11,5,3,3,5,6,7,8,8,12,15,10,4,3,4,5,6,7,8,8,12,15,10,4,3,4,5,6,7,8,8,11,15,11,5,3,4,5,6,7,8,8,10,14,12,6,3,3,4,6,7,8,8,10,13,13,8,3,3,4,6,7,7,8,9,11,15,10,4,3,3,5,6,7,8,9,10,13,13,8,4,3,4,5,7,8,8,8,11,14,12,6,3,3,4,6,7,8,8,9,12,15,10,4,3,3,4,6,7,7,8,8,12,14,10,4,3,3,4,6,7,8,8,9,12,14,10,4,3,4,5,6,7,8,8,8,11,14,11,5,3,3,4,6,7,8,8,8,10,13,12,7,3,3,4,5,7,7,8,8,9,12,14,10,4,3,3,5,6,7,8,8,8,10,13,13,8,4,3,4,5,7,7,8,8,9,10,14,12,6,3,3,4,6,7,7,8,8,8,11,14,11,6,3,3,4,6,7,8,8,8,8,10,13,12,7,3,3,4,6,7,7,8,8,8,10,12,13,9,4,3,4,5,6,7,8,8,8,9,11,13,11,5,3,3,4,6,7,8,8,8,8,9,12,13,9,4,3,3,5,6,7,8,8,8,9,9,12,13,9,4,3,4,5,6,7,8,8,8,8,9,12,13,9,4,3,4,4,6,7,8,8,8,8,9,11,13,10,5,3,3,4,6,7,7,8,8,8,8,10,12,13,8,4,3,4,5,6,7,8,8,8,8,9,10,12,12,7,4,3,4,5,7,7,8,8,8,8,8,10,12,12,7,4,3,4,5,6,7,8,8,8,8,8,9,12,12,9,4,3,4,5,6,7,8,8,8,8,8,9,10,12,11,7,3,3,4,5,7,7,8,8,8,8,9,9,10,12,11,6,3,3,4,5,6,7,8,8,8,8,8,9,10,12,12,8,4,3,4,5,6,7,8,8,8,8,8,8,9,11,12,10,5,3,3,4,6,7,7,8,8,8,8,8,8,9,11,12,9,5,3,3,4,6,7,8,8,8,8,8,8,8,9,10,12,11,6,3,3,4,5,7,7,8,8,8,8,8,8,8,9,11,12,9,5,3,3,4,5,6,7,8,8,8,8,8,8,8,8,11,12,10,6,3,3,4,5,7,7,8,8,8,8,8,8,8,8,9,11,11,8,5,3,3,5,6,7,8,8,8,8,8,8,8,8,8,9,11,12,9,5,3,3,4,6,7,7,8,8,8,8,8,8,8,8,8,9,11,11,8,4,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,9,10,11,9,6,3,3,4,6,7,7,8,8,8,8,8,8,8,8,8,8,9,10,11,9,6,3,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,9,10,11,9,5,3,3,4,6,7,8,8,8,8,8,8,8,8,8,8,8,7,8,9,11,11,8,4,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,7,8,10,11,10,7,4,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,7,8,9,10,11,9,6,4,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,7,8,8,7,9,10,10,9,7,4,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,8,8,9,10,10,8,6,4,3,4,6,7,7,8,8,8,8,8,8,8,8,8,8,8,7,8,7,7,8,7,8,9,10,10,9,7,5,4,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,8,8,7,7,8,8,9,10,10,9,6,4,4,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,7,7,7,8,8,7,8,9,9,10,9,7,5,4,4,4,6,7,7,8,8,8,8,8,8,8,8,8,8,7,7,8,8,7,7,7,7,7,7,8,7,7,7,7,8,8,9,9,9,8,6,5,4,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,8,8,7,8,7,7,8,7,7,8,7,7,7,7,7,8,7,7,8,8,8,9,9,9,9,7,6,5,4,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8,7,8,8,8,8,8,8,7,7,8,8,8,8,7,8,7,7,7,7,7,7,7,8,7,7,8,7,8,8,9,9,9,8,8,6,5,5,4,5,6,6,7,8,8,8,8,8,8,8,8,8,9,10,7,6,7,7,7,8,10,7,6,8,7,8,10,7,6,8,6,8,10,7,6,8,7,8,9,7,6,7,7,8,9,7,6,7,6,8,10,7,6,8,6,8,10,7,6,8,6,8,10,7,6,8,6,8,9,7,6,8,6,7,9,7,7,8,6,7,9,8,7,7,6,7,8,9,7,7,7,7,8,9,7,6,7,7,8,9,7,6,7,6,7,9,7,6,8,6,7,9,7,7,7,7,7,8,9,7,6,7,7,8,10,7,6,8,6,7,10,7,6,8,6,7,9,8,7,7,7,7,8,9,8,6,7,7,8,10,7,6,8,6,7,9,8,7,7,7,7,8,9,7,6,8,6,8,9,7,7,7,7,7,8,9,7,6,7,7,8,9,7,6,8,6,7,8,8,8,7,7,7,8,10,7,6,7,7,7,9,8,7,7,7,7,8,10,7,6,8,7,7,8,8,8,7,7,7,8,9,7,7,7,7,7,8,9,8,6,8,7,8,9,7,7,7,7,7,8,10,7,6,7,7,7,8,8,7,6,7,7,8,9,7,7,7,7,7,8,9,7,6,7,7,7,8,9,8,6,7,7,7,9,8,7,7,7,7,8,9,7,7,7,7,7,8,9,7,6,7,7,7,8,9,7,6,7,7,7,8,8,7,7,7,7,8,8,8,7,7,7,7,8,9,8,7,7,7,7,8,9,7,7,7,7,7,8,9,8,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,8,8,7,7,7,7,7,8,8,8,7,7,7,7,8,8,8,7,7,7,7,7,8,8,8,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,8,8,7,7,7,7,7,7,8,8,8,7,7,7,7,8,9,7,7,7,7,7,8,9,7,7,7,7,7,7,8,8,7,7,7,7,7,8,9,7,7,7,7,7,8,8,8,7,7,7,7,7,8,9,7,7,7,7,7,8,8,8,7,7,7,7,7,8,9,7,7,7,7,7,8,8,8,7,7,7,7,7,8,9,7,7,8,7,7,7,8,8,7,7,7,7,7,8,9,7,7,7,7,8,7,8,9,7,7,7,7,7,7,8,8,8,7,7,7,7,8,8,7,7,7,7,8,7,8,9,7,7,7,7,8,7,8,9,7,7,7,7,8,7,8,9,7,7,7,7,8,7,8,8,7,7,7,7,7,8,8,8,7,7,7,7,7,7,8,8,7,7,7,7,7,8,8,8,7,7,7,7,7,8,8,8,7,7,7,7,7,7,8,8,7,7,7,7,7,7,8,8,8,7,7,7,7,7,8,8,7,7,7,7,7,7,8,8,7,7,7,7,8,7,8,8,7,7,7,7,7,7,8,8,8,7,7,7,7,8,7,8,8,7,7,7,7,8,7,7,8,7,7,7,7,7,8,7,8,8,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,7,7,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,7,7,8,8,7,7,7,7,7,8,7,8,8,8,8,7,7,7,7,7,7,8,8,7,7,7,7,7,8,7,8,8,7,7,8,7,7,8,7,7,8,7,7,7,7,7,7,7,7,7,8,7,7,7,7,7,8,7,8,8,7,7,7,7,7,7,7,8,8,8,7,7,7,7,7,7,7,8,8,7,7,7,7,7,8,8,7,8,8,7,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,7,7,8,7,7,7,7,7,7,7,7,8,8,7,7,7,7,8,7,7,7,7,8,8,7,7,7,7,7,7,7,8,8,8,7,7,7,7,7,7,8,7,8,8,7,7,8,7,7,7,7,7,8,8,7,7,8,7,8,7,7,7,7,8,8,7,7,7,7,7,7,7,7,8,8,7,7,7,7,7,7,7,7,7,7,8,7,7,7,7,7,8,7,8,7,8,8,7,8,7,7,7,7,7,7,7,8,7,7,7,7,7,7,7,8,7,8,8,7,7,7,7,7,7,7,7,7,8,8,7,7,7,7,8,7,7,8,7,8,8,7,7,7,7,7,7,7,8,7,7,8,7,7,7,7,8,7,7,7,7,8,7,7,7,7,8,7,7,8,7,8,7,8,8,7,7,7,7,8,7,7,7,7,7,8,7,7,7,7,7};

const uint8_t   chord1[110] = {11,5,17,28,28,27,27,27,27,27,26,27,30,31,31,31,31,31,31,31,31,26,23,24,23,23,25,13,0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,15,28,28,27,28,26,28,27,26,30,31,31,31,31,31,31,30,23,25,25,9,3,5,4,5,5,5,5,5,6,4,6,19,29,18,5,6,5,6,6,5,20,28,25,27,25,27,23,8,4,6,5,6,6,6,6,6,6,5,12,26,29,11,10,26,26,27};

const uint8_t   octave[98] = {14,8,10,8,11,14,9,9,9,11,18,18,15,16,16,16,16,19,23,22,22,22,22,16,8,9,9,9,9,10,15,16,15,17,14,12,11,9,9,10,10,9,13,17,16,16,16,17,10,2,4,3,4,4,4,11,17,18,17,18,25,26,23,23,23,23,23,26,30,29,29,29,29,23,15,16,16,15,16,16,21,23,21,23,20,18,17,15,15,16,15,15,18,22,22,21,22,22};
// Interrupt service routine
// Executed every 12.5ns*(period)
// output one value to DAC
void SysTick_HandlerV1(void){ // write this
    if(x > 32){
        x = 0;
    }
    //DAC5_Out(SinWave[x]);
    DAC5_Out(SquareWave[x]);
    x++;
}


void SysTick_HandlerV2(void){ // write this
    if(x > 64){
        x = 0;
    }
    DAC5_Out(Trumpet[x]);
    x++;
}

void SysTick_HandlerV3(void){ // write this
    if(handlerMode == button1){
        if(x > 64){
            x = 0;
        }
        DAC5_Out(Trumpet[x]);
        x++;
    }
    if(handlerMode == button2){
            if(x > 32){
                x = 0;
            }
            DAC5_Out(SquareWave[x]);
            x++;
        }
    if(handlerMode == button3){
            if(x > 98){
                x = 0;
            }
            DAC5_Out(octave[x]);
            x++;
        }
    if(handlerMode == button4){
            if(x > 4080){
                x = 0;
            }
            DAC5_Out(shoot[x]);
            x++;
        }
}

void SysTick_Handler(void){ // write this
    if(x > 98){
        x = 0;
    }
    DAC5_Out(octave[x]);
    x++;
}
