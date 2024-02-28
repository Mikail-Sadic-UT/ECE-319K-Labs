/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Put your names here or look silly
 * :)
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>

#define SOUTH_GREEN 1
#define SOUTH_YELLOW (1<<1)
#define SOUTH_RED (1<<2)

#define WEST_GREEN (1<<6)
#define WEST_YELLOW (1<<7)
#define WEST_RED (1<<8)

#define WESTSWITCH (1<<15)
#define SOUTHSWITCH (1<<16)
#define WALKSWITCH (1<<17)

#define WALK_LED_ALL (1<<26)| (1<<27) | (1<<22)
#define WALK_LED_RED (1<<26)

// put both EIDs in the next two lines
const char EID1[] = "MFS2447"; //  ;replace abc123 with your EID
const char EID2[] = "JK49263"; //  ;replace abc123 with your EID

/* Information
EID1=MFS2447, EID2=JK49263
Option A, South,  connect Red=PB2, Yellow=PB1, Green=PB0
Option X, West,   connect Red=PB8, Yellow=PB7, Green=PB6
Option 0, Sensor, connect Walk=PB17, South=PB16, West=PB15
When all inputs true, ... South, Walk, West, South, Walk, West, South, Walk, West, ...
 */

// initialize 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B

    //South LEDs
    IOMUX->SECCFG.PINCM[PB0INDEX] = 0x0000081; //Green
    IOMUX->SECCFG.PINCM[PB1INDEX] = 0x0000081; //Yellow
    IOMUX->SECCFG.PINCM[PB2INDEX] = 0x0000081; //Red

    //West LEDs
    IOMUX->SECCFG.PINCM[PB6INDEX] = 0x0000081; //Green
    IOMUX->SECCFG.PINCM[PB7INDEX] = 0x0000081; //Yellow
    IOMUX->SECCFG.PINCM[PB8INDEX] = 0x0000081; //Red

    //Switches
    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x0040081; //West
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x0040081; //South
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x0040081; //Walk

    //Enable
    GPIOB->DOESET31_0 = SOUTH_GREEN | SOUTH_YELLOW | SOUTH_RED | WEST_GREEN | WEST_YELLOW | WEST_RED;

}
//Debugging
void LEDS_ON(void) {
    GPIOB->DOUTSET31_0 = SOUTH_GREEN | SOUTH_YELLOW | SOUTH_RED | WEST_GREEN | WEST_YELLOW | WEST_RED;
}
//Debugging
void LEDS_OFF(void) {
    GPIOB->DOUTCLR31_0 = SOUTH_GREEN | SOUTH_YELLOW | SOUTH_RED | WEST_GREEN | WEST_YELLOW | WEST_RED;
}

void WALK_ON(void) {
    GPIOB->DOUTSET31_0 = WALK_LED_ALL;
}

void WALK_RED(void) {
    GPIOB->DOUTCLR31_0 = WALK_LED_RED;
}

void WALK_OFF(void) {
    GPIOB->DOUTCLR31_0 = WALK_LED_ALL;
}

/* Activate LEDs
* Inputs: data1,data2,data3
*   specify what these means
* Output: none
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t prev, uint32_t st, uint32_t red){

    GPIOB->DOUTCLR31_0 = prev;
    GPIOB->DOUTSET31_0 = st | red;

}
/* Read sensors
 * Input: none
 * Output: sensor values
*   specify what these means
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){

    uint32_t data;
    data = (GPIOB->DIN31_0 & 0x00038000)>>15;

 return data; // return inputs
}

struct State {
    uint32_t State;
    uint32_t Out;
    uint32_t WaitTime;
    uint32_t Next[8];
};
typedef const struct State St_t;

#define goSouth 0
#define yellowSouth 1
#define waitSouth 2
#define goWest 3
#define yellowWest 4
#define waitWest 5
#define goWalk 6
#define flashRed1 7
#define flashOff1 8
#define flashRed2 9
#define flashOff2 10
#define flashRed3 11

/*states
 * 1 000 = none
 * 2 001 = south-press
 * 3 010 = west-press
 * 4 011 = south&west
 * 5 100 = walk-press
 * 6 101 = south&walk
 * 7 110 = west&walk
 * 8 111 = all
 */

#define WHITE WALK_LED_ALL
#define RED1 WALK_LED_RED
#define OFF1 WALK_LED_ALL
#define RED2 WALK_LED_RED
#define OFF2 WALK_LED_ALL
#define RED3 WALK_LED_RED


St_t FSM[12]= {
               {0, SOUTH_GREEN | WEST_RED, 200, {goSouth, goSouth, yellowSouth, yellowSouth, yellowSouth, yellowSouth, yellowSouth, yellowSouth}},
               {1, SOUTH_YELLOW | WEST_RED, 100, {waitSouth, waitSouth, waitSouth, waitSouth, waitSouth, waitSouth, waitSouth, waitSouth}},
               {2, SOUTH_RED | WEST_RED, 50, {goWalk, goWalk, goWest, goWest, goWalk, goWalk, goWalk, goWalk}},

               {3, WEST_GREEN | SOUTH_RED, 200, {goWest, yellowWest, goWest, yellowWest, yellowWest, yellowWest, yellowWest, yellowWest }},
               {4, WEST_YELLOW | SOUTH_RED, 100, {waitWest, waitWest, waitWest, waitWest, waitWest, waitWest, waitWest, waitWest}},
               {5, WEST_RED | SOUTH_RED, 50, {goWalk, goSouth, goWalk, goSouth, goWalk, goWalk, goWalk, goSouth}},

               {6, WHITE | WEST_RED | SOUTH_RED, 200, {goWalk, flashRed1, flashRed1, flashRed1, goWalk, flashRed1, flashRed1, flashRed1}},
               {7, RED1 | WEST_RED | SOUTH_RED, 50, {flashOff1, flashOff1, flashOff1, flashOff1, flashOff1, flashOff1, flashOff1, flashOff1}},
               {8, OFF1 | WEST_RED | SOUTH_RED, 50, {flashRed2, flashRed2, flashRed2, flashRed2, flashRed2, flashRed2, flashRed2, flashRed2}},
               {9, RED2 | WEST_RED | SOUTH_RED, 50, {flashOff2, flashOff2, flashOff2, flashOff2, flashOff2, flashOff2, flashOff2, flashOff2}},
               {10, OFF2 | WEST_RED | SOUTH_RED, 50, {flashRed3, flashRed3, flashRed3, flashRed3, flashRed3, flashRed3, flashRed3, flashRed3}},
               {11, RED3 | WEST_RED | SOUTH_RED, 50, {goSouth, goSouth, goWest, goWest, goSouth, goSouth, goWest, goWest}},
};

// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  UART_OutString("Lab 4, Spring 2024, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    //write debug code to test your Traffic_Out
	// Call Traffic_Out testing all LED patterns
	// Lab 3 dump to record output values
      LEDS_ON();
      Clock_Delay(40000000); // 0.5s
      LEDS_OFF();
      Clock_Delay(40000000); // 0.5s
      WALK_ON();
      Clock_Delay(40000000); // 0.5s
      WALK_RED();
      Clock_Delay(40000000); // 0.5s
      WALK_OFF();
      Clock_Delay(40000000); // 0.5s
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states

int main4(void){// main4
    uint32_t input;
    uint32_t south;
    uint32_t west;
    uint32_t walk;
    uint32_t St = goSouth;
    uint32_t prev = goSouth;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
   // initialize your FSM
  SysTick_Init();   // Initialize SysTick for software waits
  while(1){
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) hard code this so input always shows all switches pressed
    // 4) next depends on state and input
      //GPIOB->DOUT31_0 = (GPIOB->DOUT31_0&(~0x3F))|FSM[St].Out;
          Traffic_Out(FSM[prev].Out, FSM[St].Out, WALK_LED_RED);
          SysTick_Wait10ms(FSM[St].WaitTime);
          input = Traffic_In();
          south = (input & 0x02)>>1;
          west = (input & 0x01)<<1;
          walk = (input & 0x04);
          input = south | west | walk;
          prev = St;
          St = FSM[St].Next[input];
  }
}
// use main5 to grade

int main(void){// main5
    uint32_t input;
    uint32_t south;
    uint32_t west;
    uint32_t walk;
    uint32_t St = goSouth;
    uint32_t prev = goSouth;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  SysTick_Init(); // Initialize SysTick for software waits
  Debug_Init();
  // initialize your FSM
  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) input from switches 
    // 4) next depends on state and input
      Traffic_Out(FSM[prev].Out, FSM[St].Out, WALK_LED_RED);
      SysTick_Wait10ms(FSM[St].WaitTime);
      input = Traffic_In();
      south = (input & 0x02)>>1;
      west = (input & 0x01)<<1;
      walk = (input & 0x04);
      input = south | west | walk;
      prev = St;
      St = FSM[St].Next[input];
  }
}

