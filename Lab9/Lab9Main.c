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
#include "enemyHandler.h"
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

#define enemyHPdemo 42
#define enemyHPeasy 64
#define enemyHPnormal 96
#define enemyHPhard 128
#define enemyHPnohit 160

#define playerBulletBuffer 8
#define enemyBulletBuffer 64

#define ADCVREF_VDDA 0x000



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

extern uint8_t bulletCounter1, bulletCounter2, bulletCounter3;
extern uint8_t bulletTimer1, bulletTimer2, bulletTimer3;
extern uint8_t phase0, phase1, phase2, phase3, phase4, phase5;
extern uint8_t activate1, activate2, activate3;
extern uint8_t PHASE;


Entity_t thePlayer;

Entity_t theEnemy;

Entity_t playerBullet;

extern Entity_t enemyBullets[];

uint8_t WARPCounter;
uint8_t coordCounter = 0;
uint8_t switchDataOld;
uint8_t switchData;
uint8_t bossHPcounter;
uint8_t bossHPrefresh;


void TIMG12_IRQHandler(void){           //Game Engine
  if ((TIMG12->CPU_INT.IIDX) == 1){
      ADC_InDual(ADC1, &ADCX, &ADCY);   //Probes ADC
      setSpeed(&thePlayer);             //Sets Player speed based on ADC
      switchDataA = Switch_InA();       //Gets PA switch vals
      switchDataB = Switch_InB();       //Gets PB switch Vals
      switchData = SwitchHandler(switchDataA, switchDataB, &thePlayer, &playerBullet, &theEnemy); //Handles switch press
      if(GAMESTART){
          updatePlayerBulletCoords(&playerBullet, &thePlayer, &theEnemy);  //updates player bullet
          for(uint8_t i = 0; i < enemyBulletBuffer; i++) {
              if(enemyBullets[i].live) updateEnemyBulletCoords(&thePlayer, &theEnemy, i);
          }
          if(switchData == 1 && switchDataOld != 1) {
              setPlayerBulletTrajectory(&thePlayer, &playerBullet, &theEnemy); //Shoot bullet to bad guy
              switchDataOld = 1;
          } else if(switchData == 0 && switchDataOld == 1) {switchDataOld = 0;}
          updateCoords(&thePlayer);   //Updates player coord
          if(bulletHit) updateEnemyHP(&theEnemy);   //Updates enemy HP on hit
          WARPCounter++;                    //Warp timer++
          coordCounter++;                   //Coord timer++
          collisionCheck(&thePlayer, &theEnemy);    //Checks collision
          if(HPFLAG) setHPLED(&thePlayer);  //Sets HPLEDS based on player HP
          if(WARPCounter > 150){            //WARP timer ~7sec
              WARPCounter = 0;
              WARP = 1;
          }
          phaseTimer();
          Phase_Switcher(&theEnemy);
          Phase_Handler();
          Pattern_Executer(&thePlayer, &theEnemy);
          if(bossHPcounter > 5){
              bossHPrefresh = 1;
              bossHPcounter = 0;
          }
          bossHPcounter++;
      }
      //start sounds
      UPDATE = 1;                  //Update flag
  }
}

int main(void) { // main
    __disable_irq();
    PLL_Init();
    LaunchPad_Init();
    TimerG12_IntArm(80000000 / 30, 3);                // initialize interrupts on TimerG12 at 30 Hz
    __enable_irq();
    initInit();
    gameInit();                                       // data structure inits
    while (1) {
        if (UPDATE) {   //30hz
            if(GAMESTART){  //if game has started run this
                graphicsHandler(&thePlayer, &theEnemy, &playerBullet);
                drawEnemyBullets();
                if(bossHPrefresh){
                    drawIndicator();
                    bossHPrefresh = 0;
                    ST7735_SetCursor(1, 0);
                    printf("%3.3i", theEnemy.hp);
                }
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
      bulletInit(&thePlayer, &playerBullet);
      phaseInit(enemyHPeasy);
      UPDATE = 0;
      HPFLAG = 1;
      ENEMYUPDATE = 1;
      PLAYERUPDATE = 1;
      GAMEOVER = 0;
      CRASH = 0;
      bulletHit = 0;
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
      WARPCounter = 0;
      switchDataOld = 0;
      PHASE = 0;
      activate1 = 0;
      activate2 = 0;
      activate3 = 0;
      bossHPcounter = 0;
      bossHPrefresh = 0;
      for(uint8_t i = 0; i < enemyBulletBuffer; i++) {
          bulletReset(&theEnemy, i);
      }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));}
void initInit(){
    ST7735_InitPrintf();
    ST7735_FillScreen(ST7735_BLACK);
    ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);           // init Dual ADC
    Switch_Init();                                    // initialize switches
    LED_Init();                                       // initialize LED
    Sound_Init();                                     // initialize sound
    TExaS_Init(0, 0, &TExaS_LaunchPadLogicPB27PB26);  // PB27 and PB26
}

void PLL_Init(void){Clock_Init80MHz(0);}

uint32_t M = 1;
uint32_t Random32(void){
  M = 1664525 * M + 1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32() >> 16) % n;
}

// use main4 to test sound outputs
int main1(void)
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
    now = Switch_InA(); // one of your buttons
    if ((last == 0) && (now == UP))
    {
      Sound_Crash(); // call one of your sounds
    }
    if ((last == 0) && (now == LFT))
    {
      Sound_Warp(); // call one of your sounds
    }
    if ((last == 0) && (now == RT))
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

