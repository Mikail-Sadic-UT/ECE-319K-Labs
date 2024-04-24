// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Mikail Sadic & Joshua Koshy
// Last Modified: 4/22/2024

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
                             /*   IMPORTANT!!!  For code to run, need to use updated ST7735.c and ST7735.h (will only work for horizontal screen)   */
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
#define enemyHPeasy 67
#define enemyHPnormal 77
#define enemyHPhard 87
#define enemyHPnohit 97

#define playerBulletBuffer 8
#define enemyBulletBuffer 64

#define ADCVREF_VDDA 0x000

#define Demo 0
#define Easy 1
#define Norm 2
#define Hard 3
#define NoHi 4

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
uint8_t INFO;
uint8_t score;
uint8_t FIRSTUPDATE;
uint8_t CONTROLS;
uint8_t LORE;
uint8_t LOREUPDATE;
uint8_t PAUSED;
uint8_t UNPAUSED;
int8_t pauseCount;
uint8_t Mode;

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
uint8_t switchDataOld;
uint8_t switchData;
uint8_t refreshCounter;
uint8_t refresh;

#define HIT 1
#define HITP 3000

#define Crash 2
#define CRASHP 5000

#define Warp 3
#define WARPP 5000

void TIMG12_IRQHandler(void){           //Game Engine
  if ((TIMG12->CPU_INT.IIDX) == 1){
      ADC_InDual(ADC1, &ADCX, &ADCY);   //Probes ADC
      setSpeed(&thePlayer);             //Sets Player speed based on ADC
      switchDataA = Switch_InA();       //Gets PA switch vals
      switchDataB = Switch_InB();       //Gets PB switch Vals
      switchData = SwitchHandler(switchDataA, switchDataB, &thePlayer, &playerBullet, &theEnemy); //Handles switch press
      if(GAMESTART && switchData == 4 && switchDataOld != 4 && pauseCount > 0){
          UNPAUSED = 1;
          switchDataOld = 4;
          if(PAUSED) {                                   //Pause handler
              PAUSED = 0;
              pauseCount--;
          } else PAUSED = 1;
      }
      if(GAMESTART && !PAUSED){                                             //Run game
          updatePlayerBulletCoords(&playerBullet, &thePlayer, &theEnemy);   //updates player bullet
          for(uint8_t i = 0; i < enemyBulletBuffer; i++) {
              if(enemyBullets[i].live) updateEnemyBulletCoords(&thePlayer, &theEnemy, i);   //Update enemy bullet coords
          }
          if(switchData == 1) { // && switchDataOld != 1   <--- for button mash
              setPlayerBulletTrajectory(&thePlayer, &playerBullet, &theEnemy);              //Shoot bullet to bad guy
              //switchDataOld = 1;    <--- for button mash
          } else if(switchData == 0 && switchDataOld == 1) switchDataOld = 0;
          updateCoords(&thePlayer);                         //Updates player coord
          updateIndicatorCoords(&thePlayer);
          if(bulletHit) updateEnemyHP(&theEnemy);           //Updates enemy HP on hit
          collisionCheck(&thePlayer, &theEnemy);            //Checks collision
          if(HPFLAG) setHPLED(&thePlayer);                  //Sets HPLEDS based on player HP
          WARPCounter++;
          if(WARPCounter > 150){
              WARPCounter = 0;                              //WARP timer ~5sec
              WARP = 1;
          }
          phaseTimer();                                     //Attack pattern clocks
          Phase_Switcher(&theEnemy);                        //Switched phase based off HP
          Phase_Handler();                                  //Sets timers and activates patterns
          Pattern_Executer(&thePlayer, &theEnemy);          //executes pattern
          if(refreshCounter > 5){
              refresh = 1;                                  //Refreshes bossHP indicator and warp indicator at ~5hz
              refreshCounter = 0;
          }
          refreshCounter++;
      }
      switchDataOld = switchData;
      UPDATE = 1;                           //Update flag
  }
}

int main(void) {            // Main
    __disable_irq();
    PLL_Init();
    LaunchPad_Init();
    TimerG12_IntArm(80000000 / 30, 3);                // Initialize interrupts on TimerG12 at 30 Hz
    __enable_irq();
    initInit();                                       // Hardware inits
    gameInit();                                       // Data structure inits
    while (1) {
        if (UPDATE) {   //30hz
            if(GAMESTART && !PAUSED){                                   // If game has started run this
                if(UNPAUSED) clearPause();                              // Clears pause menu
                graphicsHandler(&thePlayer, &theEnemy, &playerBullet);  // Handles most graphics
                drawEnemyBullets();                                     // Handles enemy bullet graphics
                if(refresh) refreshUI(&theEnemy);                       // Refreshing ingame ui
            } else if(GAMESTART && PAUSED){
                pauseHandler(pauseCount);                               // Handles pause menu
            } else {
                menuHandler(&thePlayer, &theEnemy);                     // Handles menus
            }
            UPDATE = 0;         // flag reset
        }
        while(GAMEOVER) gameEndHandler(&thePlayer);                     // Game end handlers
        while(WIN) winHandler(&theEnemy);
    }
}

void gameInit(){                                        // Game init
    ST7735_FillScreen(ST7735_BLACK);
      playerInit(&thePlayer, playerHPeasy);             // Inits player
      enemyInit(&theEnemy, enemyHPeasy);                // Inits enemy
      bulletInit(&thePlayer, &playerBullet);            // Inits bullet
      phaseInit(enemyHPeasy);                           // Sets Phase HPs
      setHPLED(&thePlayer);                             // Sets LEDS
      Mode = 1;
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
      score = 0;                                        // Basically resets all global flags
      FIRSTUPDATE = 1;
      LORE = 0;
      CONTROLS = 0;
      LOREUPDATE = 0;
      WARPCounter = 0;
      WARP = 0;
      switchDataOld = 0;
      PHASE = 0;
      activate1 = 0;
      activate2 = 0;
      activate3 = 0;
      refreshCounter = 0;
      refresh = 0;
      PAUSED = 0;
      UNPAUSED = 0;
      INFO = 0;
      pauseCount = 3;
      for(uint8_t i = 0; i < enemyBulletBuffer; i++) {
          bulletReset(&theEnemy, i);                    // Inits enemy bullets
      }
}


void reset(){                               // Resets game to main menu without resetting everything (Keeps language and current gamemode)
    ST7735_FillScreen(ST7735_BLACK);
    if(Mode == Demo){
        playerInit(&thePlayer, playerHPdemo);             // Inits player
        enemyInit(&theEnemy, enemyHPdemo);                // Inits enemy
        bulletInit(&thePlayer, &playerBullet);            // Inits bullet
        phaseInit(enemyHPdemo);                           // Sets Phase HPs
        setHPLED(&thePlayer);                             // Sets LEDS
    }
    else if(Mode == Easy){
        playerInit(&thePlayer, playerHPeasy);
        enemyInit(&theEnemy, enemyHPeasy);
        bulletInit(&thePlayer, &playerBullet);
        phaseInit(enemyHPeasy);
        setHPLED(&thePlayer);
    }
    else if(Mode == Norm){
        playerInit(&thePlayer, playerHPnormal);
        enemyInit(&theEnemy, enemyHPnormal);
        bulletInit(&thePlayer, &playerBullet);
        phaseInit(enemyHPnormal);
        setHPLED(&thePlayer);
    }
    else if(Mode == Hard){
        playerInit(&thePlayer, playerHPhard);
        enemyInit(&theEnemy, enemyHPhard);
        bulletInit(&thePlayer, &playerBullet);
        phaseInit(enemyHPhard);
        setHPLED(&thePlayer);
    }
    else if(Mode == NoHi){
        playerInit(&thePlayer, playerHPnohit);
        enemyInit(&theEnemy, enemyHPnohit);
        bulletInit(&thePlayer, &playerBullet);
        phaseInit(enemyHPnohit);
        setHPLED(&thePlayer);
    }
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
         LANGSELECT = 0;
         OPTIONSELECT = 0;
         MAINMENU = 1;
         score = 0;                                        // Basically resets all global flags
         FIRSTUPDATE = 1;
         LORE = 0;
         CONTROLS = 0;
         LOREUPDATE = 0;
         WARPCounter = 0;
         WARP = 0;
         switchDataOld = 0;
         PHASE = 0;
         activate1 = 0;
         activate2 = 0;
         activate3 = 0;
         refreshCounter = 0;
         refresh = 0;
         PAUSED = 0;
         UNPAUSED = 0;
         INFO = 0;
         pauseCount = 3;
         for(uint8_t i = 0; i < enemyBulletBuffer; i++) {
             bulletReset(&theEnemy, i);                    // Inits enemy bullets
         }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void) {return (0x80 | ((GPIOB->DOUT31_0 >> 26) & 0x03));}
void initInit(){                                      // A bunch of other inits
    ST7735_InitPrintf();
    ST7735_FillScreen(ST7735_BLACK);
    ADC_InitDual(ADC1, 4, 6, ADCVREF_VDDA);           // Init Dual ADC
    Switch_Init();                                    // Initialize switches
    LED_Init();                                       // Initialize LED
    Sound_Init();                                     // Initialize sound
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
