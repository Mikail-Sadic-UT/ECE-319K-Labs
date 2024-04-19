#include <enemyHandler.h>
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
#include "Graphics.h"
#include "menuHandler.h"

#define R3D (1 << 17)
#define YEL (1 << 28)
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

#define enemyHPdemo 64
#define enemyHPeasy 96
#define enemyHPnormal 128
#define enemyHPhard 192
#define enemyHPnohit 255

extern uint8_t LANGMODE;
extern uint32_t switchDataA;
extern uint32_t switchDataB;
extern uint8_t OPTIONSELECT;
extern uint8_t MAINMENU;
extern uint16_t score;
extern uint8_t FIRSTUPDATE;
extern uint8_t CONTROLS;
extern uint8_t LORE;
extern uint8_t LOREUPDATE;
extern uint8_t LANGSELECT;
extern uint8_t GAMESTART;
extern uint8_t CONTINUE;


void menuHandler(Entity_t *thePlayer, Entity_t *theEnemy){
    if(LANGSELECT) langSelect();
    if(MAINMENU) mainMenu();
    if(OPTIONSELECT) Options(thePlayer, theEnemy);
    if(CONTROLS) controls();
    if(LORE) lore();
}

void langSelect(){
    drawLangScrn();
    if((switchDataA&UP) == UP){
        LANGMODE = 1; //ENG
        LANGSELECT = 0;
        CONTROLS = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){
        LANGMODE = 2; //BH
        LANGSELECT = 0;
        CONTROLS = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void mainMenu(){
    if(LANGMODE == 1) drawTitleEng();
    if(LANGMODE == 2) drawTitleBH();

    if((switchDataA&RT) == RT){
        GAMESTART = 1;
        MAINMENU = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){
        OPTIONSELECT = 1;
        MAINMENU = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&LFT) == LFT){
        CONTROLS = 1;
        MAINMENU = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
        }
    if((switchDataA&UP) == UP){
        LORE = 1;
        MAINMENU = 0;
        LOREUPDATE = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void Options(Entity_t *thePlayer, Entity_t *theEnemy){  //Sets difficulty (playerHP and enemyHP)
    if(LANGMODE == 1) drawEngOpt();
    if(LANGMODE == 2) drawBHOpt();

    if((switchDataA&UP) == UP){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPdemo);             // inits player
        enemyInit(theEnemy, enemyHPdemo);               // inits enemy
        phaseInit(enemyHPdemo);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&RT) == RT){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPeasy);             // inits player
        enemyInit(theEnemy, enemyHPeasy);               // inits enemy
        phaseInit(enemyHPeasy);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPnormal);             // inits player
        enemyInit(theEnemy, enemyHPnormal);               // inits enemy
        phaseInit(enemyHPnormal);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&LFT) == LFT){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPhard);             // inits player
        enemyInit(theEnemy, enemyHPhard);               // inits enemy
        phaseInit(enemyHPhard);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataB&SP) == SP){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPnohit);             // inits player
        enemyInit(theEnemy, enemyHPnohit);               // inits enemy
        phaseInit(enemyHPnohit);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void controls(){
    if(LANGMODE == 1) controlsEng();
    if(LANGMODE == 2) controlsBH();
    if(switchDataA > 0){
        MAINMENU = 1;
        CONTROLS = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void lore(){
    if(LOREUPDATE){
        if(LANGMODE == 1) loreEng();
        if(LANGMODE == 2) loreBH();
    }
    if(switchDataA > 0){
        MAINMENU = 1;
        LORE = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    LOREUPDATE = 0;
}

void win(){
    if(LANGMODE == 1) winEng();
    if(LANGMODE == 2) winBH();
}

void answers(){
    Clock_Delay1ms(250);
    if(LANGMODE == 1) answersEng();
    if(LANGMODE == 2) answersBH();
}

void lose(){
    if(LANGMODE == 1) gameOverEng();
    if(LANGMODE == 2) gameOverBH();
}

/*
PauseMenu(){  When paused, go here until unpause
  Options:
  Resume
  Quit
}
*/
