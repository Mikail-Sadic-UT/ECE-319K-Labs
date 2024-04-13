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
#include "Engine.h"
#include "Lab9Main.h"
#include "menuHandler.h"
#include "Graphics.h"
                                                    //IMPORTANT!!!  For code to run, need to use updated ST7735.c and ST7735.h (will only work for horizontal screen)
#define R3D (1 << 17)
#define YEL (1 << 28) // Info
#define GRN (1 << 31)

#define UP (1<<24)
#define DWN (1<<26)
#define LFT (1<<25)
#define RT (1<<27)
#define SP (1<<16)

#define playerHPdemo 13
#define playerHPeasy 7
#define playerHPnormal 5
#define playerHPhard 3
#define playerHPnohit 1

#define enemyHPdemo 15
#define enemyHPeasy 30
#define enemyHPnormal 45
#define enemyHPhard 60
#define enemyHPnohit 75

#define playerBulletBuffer 15
#define enemyBulletBuffer 255

void PLL_Init(void){Clock_Init80MHz(0);}

uint32_t M = 1;
uint32_t Random32(void){
  M = 1664525 * M + 1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32() >> 16) % n;
}

uint8_t GAMEMODE;

uint32_t ADCX;
uint32_t ADCY;

uint32_t switchDataA;
uint32_t switchDataB;

uint8_t UPDATE;
uint8_t HPFLAG;
uint8_t GAMEOVER;
uint8_t WARP;
uint8_t PLAYERUPDATE;
uint8_t ENEMYUPDATE;
uint8_t CRASH;
uint8_t bulletHit;
uint8_t bulletLive;
uint8_t lastClear;
uint8_t WIN;
uint8_t GAMESTART;
uint8_t LANGSELECT;
uint8_t LANGMODE;
uint8_t OPTIONSELECT;
uint8_t MAINMENU;
uint16_t score;
uint8_t FIRSTUPDATE;
uint8_t CONTROLS;
uint8_t LORE;
uint8_t LOREUPDATE;


Entity_t thePlayer;

Entity_t theEnemy;

Entity_t playerBullet;

Entity_t enemyBullets;

uint8_t WARPCounter = 0;
uint8_t coordCounter = 0;
uint8_t switchData;

void TIMG12_IRQHandler(void){           //Game Engine
  if ((TIMG12->CPU_INT.IIDX) == 1){
      ADC_InDual(ADC1, &ADCX, &ADCY);   //Probes ADC
      setSpeed(&thePlayer);             //Sets Player speed based on ADC
      switchDataA = Switch_InA();       //Gets PA switch vals
      switchDataB = Switch_InB();       //Gets PB switch Vals
      switchData = SwitchHandler(switchDataA, switchDataB, &thePlayer); //Handles switch press
      if(GAMESTART){
          if(switchData == 1) setPlayerBulletTrajectory(&thePlayer, &playerBullet); //Shoot bullet to bad guy
          updateCoords(&thePlayer);   //Updates player coord at 15hz
          updatePlayerBulletCoords(&playerBullet, &thePlayer);  //updates player bullet
          if(bulletHit) updateEnemyHP(&theEnemy);   //Updates enemy HP on hit
          WARPCounter++;                    //Warp timer++
          coordCounter++;                   //Coord timer++
          if(WARPCounter > 254){            //WARP timer ~7sec
              WARPCounter = 0;
              WARP = 1;
          }
          collisionCheck(&thePlayer, &theEnemy);    //Checks collision
          if(HPFLAG) setHPLED(&thePlayer);  //Sets HPLEDS based on player HP
      }
      //start sounds
      UPDATE = 1;                       //Update flag
  }
}

/*

create linked list of bullets
add to linked list when creating a bullet
remove from linked list when destroying a bullet

every frame, loop through the list and check:
1. Does the bullet need to be destroyed?
    a. Yes, out of range (destroy)
    b. Yes, hit player (take care of what needs to be taken care of, destroy)
2. Apply speed vectors to bullet coord
3. Draw bullet
4. Go to next item in linked list (until null)

*/

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));}
#define ADCVREF_VDDA 0x000

int main(void) { // main
    __disable_irq();
    PLL_Init();
    LaunchPad_Init();
    TimerG12_IntArm(80000000 / 30, 2);                // initialize interrupts on TimerG12 at 30 Hz
    __enable_irq();
    initInit();
    gameInit();                                       // data structure inits
    while (1) {
        if (UPDATE) {   //30hz
            if(GAMESTART){  //if game has started run this
                graphicsHandler(&thePlayer, &theEnemy, &playerBullet);
            } else {    //else do menus
                menuHandler(&thePlayer, &theEnemy);
            }
            UPDATE = 0;
        }
        while(GAMEOVER) gameEndHandler();
        while(WIN) winHandler();
    }
}

void gameInit(){ // Flag inits
    ST7735_FillScreen(ST7735_BLACK);
      playerInit(&thePlayer, playerHPeasy);             // inits player
      enemyInit(&theEnemy, enemyHPeasy);               // inits enemy
      UPDATE = 0;
      HPFLAG = 1;
      ENEMYUPDATE = 1;
      PLAYERUPDATE = 1;
      GAMEOVER = 0;
      CRASH = 0;
      bulletHit = 0;
      bulletLive = 0;
      lastClear = 0;
      WIN = 0;
      GAMESTART = 0;
      LANGSELECT = 1;
      OPTIONSELECT = 0;
      LANGMODE = 0;
      MAINMENU = 0;
      score = 0;
      FIRSTUPDATE = 1;
      LORE = 0;
      CONTROLS = 0;
      LOREUPDATE = 0;
}

void initInit(){
    ST7735_InitPrintf();
    ST7735_FillScreen(ST7735_BLACK);
    ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);           // init Dual ADC
    Switch_Init();                                    // initialize switches
    LED_Init();                                       // initialize LED
    Sound_Init();                                     // initialize sound
    TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26);  // PB27 and PB26
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
  playerInit(&thePlayer, playerHPdemo);
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
      printf("WARPAvail: %1i", WARP);
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
  ST7735_FillScreen(ST7735_BLACK);
  while (1)
  {
      drawLangScrn();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      drawTitleEng();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      drawTitleBH();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      drawEngOpt();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      drawBHOpt();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      gameOverEng();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      gameOverBH();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      winEng();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
      winBH();
      Clock_Delay1ms(1000);
      ST7735_FillScreen(ST7735_BLACK);
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


int main89(void)
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
      //Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN

