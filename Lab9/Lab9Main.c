// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Engine.h"

#define R3D (1 << 17)
#define YEL (1 << 28) // Info
#define GRN (1 << 31)

#define UP (1<<24)
#define DWN (1<<26)
#define LFT (1<<25)
#define RT (1<<27)
#define SP (1)//change later lol

void PLL_Init(void){Clock_Init80MHz(0);} // run this line for 80MHz

uint32_t M = 1;
uint32_t Random32(void){
  M = 1664525 * M + 1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32() >> 16) % n;
}

uint8_t GAMEMODE;

uint32_t *ADCX;
uint32_t *ADCY;

uint32_t switchDataA;
uint32_t switchDataB;

uint8_t UPDATE;
uint8_t HPFLAG;
uint8_t DASH;
uint8_t PLAYERUPDATE;

Player_t thePlayer;

Enemy_t thEnemy;

uint8_t dashCounter = 0;
uint8_t coordCounter = 0;

uint8_t switchData;
// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){ // game engine goes here
  if ((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
      ADC_InDual(ADC1, &ADCX, &ADCY);
      setSpeed(&thePlayer);
      switchDataA = Switch_InA(); // 2) read input switches
      switchDataB = Switch_InB();
      switchData = SwitchHandler(switchDataA, switchDataB, &thePlayer);
      if(coordCounter == 2){
          updateCoords(&thePlayer);
          coordCounter = 0;
      }
      dashCounter++;
      coordCounter++;
      if(dashCounter > 254){ //Dash timer ~7sec
          dashCounter = 0;
          DASH = 1;
      }
      // 3) move sprites0
      // 4) start sounds
      UPDATE = 1; // 5) set semaphore
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));}
#define ADCVREF_VDDA 0x000

int main(void) { // main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);
  Switch_Init();                                   // initialize switches
  LED_Init();                                      // initialize LED
  Sound_Init();                                    // initialize sound
  TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  TimerG12_IntArm(80000000 / 30, 2);               // initialize interrupts on TimerG12 at 30 Hz
  // initialize all data structures
  playerInit(&thePlayer);
  __enable_irq();
  UPDATE = 0;
  int16_t spdx, spdy, x, y, xOld, yOld;
  while (1)
  {
    if (UPDATE)
    {             // wait for semaphore
      UPDATE = 0; // clear semaphore
                  // update ST7735R
      spdx = thePlayer.spdX;
      spdy = thePlayer.spdY;
      x = thePlayer.x;
      y = thePlayer.y;
      if(PLAYERUPDATE){
          ST7735_DrawBitmap(yOld, xOld, playerOld, 11, 11);
          ST7735_DrawBitmap(y, x, player, 11, 11);
      }
      xOld = x;
      yOld = y;
    }

    // check for end game or level switch
  }
}

int mainDebug(void) { // Debug main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);
  Switch_Init();                                   // initialize switches
  LED_Init();                                      // initialize LED
  Sound_Init();                                    // initialize sound
  TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  TimerG12_IntArm(80000000 / 30, 2);               // initialize interrupts on TimerG12 at 30 Hz
  // initialize all data structures
  playerInit(&thePlayer);
  __enable_irq();
  UPDATE = 0;
  int16_t spdx, spdy, x, y, xOld, yOld;
  while (1)
  {
    if (UPDATE)
    {             // wait for semaphore
      UPDATE = 0; // clear semaphore
                  // update ST7735R
      spdx = thePlayer.spdX;
      spdy = thePlayer.spdY;
      x = thePlayer.x;
      y = thePlayer.y;
      ST7735_SetCursor(0,0);
      printf("SpdX = %2i", spdx);
      ST7735_SetCursor(0,1);
      printf("SpdY = %2i", spdy);
      ST7735_SetCursor(0,2);
      printf("X = %4i", x);
      ST7735_SetCursor(0,3);
      printf("Y = %4i", y);
      ST7735_SetCursor(0,4);
      printf("Button: %2i", switchData);
      ST7735_SetCursor(0,5);
      printf("DashAvail: %1i", DASH);
    }

    // check for end game or level switch
  }
}


typedef enum
{
  English,
  Spanish,
  Portuguese,
  French
} Language_t;
Language_t myLanguage = English;
typedef enum
{
  HELLO,
  GOODBYE,
  LANGUAGE
} phrase_t;
const char Hello_English[] = "Hello";
const char Hello_Spanish[] = "\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] = "All\x83";
const char Goodbye_English[] = "Goodbye";
const char Goodbye_Spanish[] = "Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[] = "English";
const char Language_Spanish[] = "Espa\xA4ol";
const char Language_Portuguese[] = "Portugu\x88s";
const char Language_French[] = "Fran\x87"
                               "ais";
const char *Phrases[3][4] = {
    {Hello_English, Hello_Spanish, Hello_Portuguese, Hello_French},
    {Goodbye_English, Goodbye_Spanish, Goodbye_Portuguese, Goodbye_French},
    {Language_English, Language_Spanish, Language_Portuguese, Language_French}};
// use main1 to observe special characters
int main1(void)
{ // main1
  char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000); // set screen to black
  for (phrase_t myPhrase = HELLO; myPhrase <= GOODBYE; myPhrase++)
  {
    for (Language_t myL = English; myL <= French; myL++)
    {
      ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
      ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000); // set screen to black
  l = 128;
  while (1)
  {
    Clock_Delay1ms(2000);
    for (int j = 0; j < 3; j++)
    {
      for (int i = 0; i < 16; i++)
      {
        ST7735_SetCursor(7 * j + 0, i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7 * j + 4, i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void)
{ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  // note: if you colors are weird, see different options for
  //  ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  /*
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18, 8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18, 5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18, 8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18, 8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18, 8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16, 10);
  ST7735_DrawBitmap(20, 9, SmallEnemy10pointB, 16, 10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16, 10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16, 10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16, 10);
  */
  ST7735_DrawBitmap(96, 100, red_Bullet, 5, 5);
  ST7735_DrawBitmap(32, 100, blue_Bullet, 5, 5);
  ST7735_DrawBitmap(64, 100, player_Bullet, 2, 2);
  ST7735_DrawBitmap(59, 128, player, 11, 11);
  ST7735_DrawBitmap(52, 90, enemy, 25, 25);
  for (uint32_t t = 500; t > 0; t = t - 5)
  {
    SmallFont_OutVertical(t, 104, 6); // top left
    Clock_Delay1ms(50);               // delay 50 msec
  }
  ST7735_FillScreen(0x0000); // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Peanits!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while (1)
  {
  }
}



// use main3 to test switches and LEDs
int main3(void)
{ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  while (1)
  {
      uint8_t up, dwn, lft, rt;
      uint32_t SwitchData;
      SwitchData = Switch_InA();
      if(SwitchData == UP){
          up++;
          LED_On(R3D);
          LED_Off(GRN | YEL);
      }
      if(SwitchData == DWN){
                dwn++;
                LED_On(YEL);
                LED_Off(GRN | R3D);
            }
      if(SwitchData == LFT){
                lft++;
                LED_On(GRN);
                LED_Off(YEL | R3D);
            }
      if(SwitchData == RT){
          LED_On(GRN | R3D);
          LED_Off(YEL);
                rt++;
            }
  }
}


int main0(void)
{
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  DAC5_Init();
  ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);
  uint32_t x, y;
  while (1)
  {

      ADC_InDual(ADC1, &ADCX, &ADCY);

      x = ADCX;
      y = ADCY;
      x = x >> 8;
      y = y >> 8;
      x = x - 8;
      y = y - 8;

      //x = ADCin();
      ST7735_SetCursor(0,0);
      printf("%4i", x);
      ST7735_SetCursor(0,1);
      printf("%4i", y);

  }
}
// use main4 to test sound outputs
int main4(void)
{
  uint32_t last = 0, now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init();          // initialize switches
  LED_Init();             // initialize LED
  Sound_Init();           // initialize sound
  TExaS_Init(ADC0, 6, 0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while (1)
  {
    now = Switch_In(); // one of your buttons
    if ((last == 0) && (now == 1))
    {
      Sound_Shoot(); // call one of your sounds
    }
    if ((last == 0) && (now == 2))
    {
      Sound_Killed(); // call one of your sounds
    }
    if ((last == 0) && (now == 4))
    {
      Sound_Explosion(); // call one of your sounds
    }
    if ((last == 0) && (now == 8))
    {
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN

