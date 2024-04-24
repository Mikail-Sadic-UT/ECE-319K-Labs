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

#define enemyHPdemo 42
#define enemyHPeasy 67
#define enemyHPnormal 77
#define enemyHPhard 87
#define enemyHPnohit 97

#define Crash 2
#define CRASHP 5000
#define TelangHP 127

#define Demo 0
#define Easy 1
#define Norm 2
#define Hard 3
#define NoHi 4

extern uint8_t LANGMODE;
extern uint32_t switchDataA;
extern uint32_t switchDataB;
extern uint8_t OPTIONSELECT;
extern uint8_t MAINMENU;
extern uint8_t score;
extern uint8_t FIRSTUPDATE;
extern uint8_t CONTROLS;
extern uint8_t LORE;
extern uint8_t INFO;
extern uint8_t LOREUPDATE;
extern uint8_t LANGSELECT;
extern uint8_t GAMESTART;
extern uint8_t CONTINUE;
extern uint8_t Mode;
uint8_t TELANG;
uint8_t TelangMode;

void menuHandler(Entity_t *thePlayer, Entity_t *theEnemy){  //Handles all menus
    if(LANGSELECT) langSelect();
    if(MAINMENU) mainMenu();
    if(OPTIONSELECT) Options(thePlayer, theEnemy);
    if(CONTROLS) controls(thePlayer, theEnemy);
    if(LORE) lore();
    if(INFO) info();
    if(TELANG) Telang();
}

void pauseHandler(int8_t pauseCount){   //handles pauses
    ST7735_SetCursor(8, 2);
    if(LANGMODE == 1) ST7735_OutStringCool("PAUSED", 2, ST7735_WHITE);
    if(LANGMODE == 2) {
        ST7735_SetCursor(2, 2);
        ST7735_OutStringCool("ZAUSTAVLJENO", 2, ST7735_WHITE);
    }
    ST7735_SetCursor(7, 9);
    if(LANGMODE == 1) ST7735_OutStringCool("Pauses left: ", 1, ST7735_ORANGE);
    if(LANGMODE == 2){
        ST7735_SetCursor(8, 9);
        ST7735_OutStringCool("Zaustave: ", 1, ST7735_ORANGE);
    }
    printf("%i", pauseCount);
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
    if((switchDataB&SP) == SP){
        INFO = 1;
        MAINMENU = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void Options(Entity_t *thePlayer, Entity_t *theEnemy){  //Sets difficulty (playerHP and enemyHP)
    if(LANGMODE == 1) drawEngOpt();
    if(LANGMODE == 2) drawBHOpt();

    if((switchDataA&UP) == UP){                         //DEMO
        Mode = Demo;
        OPTIONSELECT = 0;
        MAINMENU = 1;
        TelangMode = 0;
        playerInit(thePlayer, playerHPdemo);            // inits player
        enemyInit(theEnemy, enemyHPdemo);               // inits enemy
        phaseInit(enemyHPdemo);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&RT) == RT){                         //EASY
        Mode = Easy;
        OPTIONSELECT = 0;
        MAINMENU = 1;
        TelangMode = 0;
        playerInit(thePlayer, playerHPeasy);            // inits player
        enemyInit(theEnemy, enemyHPeasy);               // inits enemy
        phaseInit(enemyHPeasy);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){                       //NORMAL
        Mode = Norm;
        OPTIONSELECT = 0;
        MAINMENU = 1;
        TelangMode = 0;
        playerInit(thePlayer, playerHPnormal);          // inits player
        enemyInit(theEnemy, enemyHPnormal);             // inits enemy
        phaseInit(enemyHPnormal);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&LFT) == LFT){                       //HARD
        Mode = Hard;
        OPTIONSELECT = 0;
        MAINMENU = 1;
        TelangMode = 0;
        playerInit(thePlayer, playerHPhard);            // inits player
        enemyInit(theEnemy, enemyHPhard);               // inits enemy
        phaseInit(enemyHPhard);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataB&SP) == SP){                         //NO-HIT
        Mode = NoHi;
        OPTIONSELECT = 0;
        MAINMENU = 1;
        TelangMode = 0;
        playerInit(thePlayer, playerHPnohit);           // inits player
        enemyInit(theEnemy, enemyHPnohit);              // inits enemy
        phaseInit(enemyHPnohit);
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void controls(Entity_t *thePlayer, Entity_t *theEnemy){
    if(LANGMODE == 1) controlsEng();
    if(LANGMODE == 2) controlsBH();
    if(switchDataA > 0){
        MAINMENU = 1;
        CONTROLS = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if(switchDataB > 0) {
        Sound_Start(CRASHP, Crash);
        playerInit(thePlayer, playerHPnohit);           // inits player
        enemyInit(theEnemy, TelangHP);              // inits enemy
        phaseInit(TelangHP);
        TELANG = 1;
        TelangMode = 1;
        CONTROLS = 0;
    }
}

void Telang(){
    drawTelang();
    if(switchDataA > 0){
        MAINMENU = 1;
        TELANG = 0;
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

void info(){
    if(LANGMODE == 1) infoEng();
    if(LANGMODE == 2) infoBH();
    if(switchDataA > 0 || switchDataB > 0){
        MAINMENU = 1;
        INFO = 0;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
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
