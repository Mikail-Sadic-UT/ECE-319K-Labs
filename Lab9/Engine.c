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
#define YEL (1 << 28)
#define GRN (1 << 31)

#define UP (1<<24)
#define DWN (1<<26)
#define LFT (1<<25)
#define RT (1<<27)
#define SP (1<<16)



extern uint32_t ADCX;
extern uint32_t ADCY;
extern uint8_t HPFLAG;
extern uint8_t WARP;
extern uint8_t PLAYERUPDATE;
extern uint8_t ENEMYUPDATE;
extern uint8_t coordCounter;
extern uint8_t GAMEOVER;
extern uint8_t CRASH;
extern uint8_t bulletHit;
extern int8_t bulletLive;
extern uint8_t lastClear;
extern uint8_t WIN;
extern uint8_t WIN;
extern uint8_t GAMESTART;
extern uint8_t LANGSELECT;
extern uint8_t LANGMODE;
extern uint8_t OPTIONSELECT;
extern uint32_t switchDataA;
extern uint32_t switchDataB;
extern uint8_t OPTIONSELECT;
extern uint8_t MAINMENU;
extern uint16_t score;
extern uint8_t FIRSTUPDATE;
extern uint8_t CONTROLS;
extern uint8_t LORE;
extern uint8_t LOREUPDATE;

void playerInit(Entity_t *thePlayer, uint16_t hp){  //Initializes player
    thePlayer->x = 128;
    thePlayer->y = 59;
    thePlayer->hp = hp;
    thePlayer->h = 11;
    thePlayer->w = 11;
}

void enemyInit(Entity_t *theEnemy, uint16_t hp){    //Initializes enemy
    theEnemy->x = 93;
    theEnemy->y = 51;
    theEnemy->hp = hp;
    theEnemy->h = 25;
    theEnemy->w = 25;
}

void setSpeed(Entity_t *thePlayer)  //sets Player speed
{
  int32_t X = ADCX;
  int32_t Y = ADCY;
  X = X >> 9;
  Y = Y >> 9;

    X = X - 4; // X speed is -3 to +3
    if(X < 0) X++;

    Y = Y - 4; // Y speed is -3 to +3
    if(Y < 0) Y++;

    Y = Y*-1;  //corrects Y axis
    thePlayer->spdX = X;
    thePlayer->spdY = Y;
}


void updateCoords(Entity_t *Entity){    //Updates coord of entity
    if(Entity->w == 11) updatePlayerCoords(Entity);
    if(Entity->w == 5) updateEnemyBulletCoords(Entity);
    if(Entity->w == 25) updateEnemyCoords(Entity); //Prob will not be implemented
}

uint32_t collisioncheck = 0;
void updatePlayerCoords(Entity_t *Entity){
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    spdX = Entity->spdX;
    spdY = Entity->spdY;
    if((spdX != 0) || (spdY != 0)){ //update detection
        xOld = Entity->x;
        yOld = Entity->y;

        x = (xOld + spdX);
        if(x > 160) x = 160;    //Out of bounds detection
        if(x < 10) x = 10;

        y = (yOld + spdY);
        if(y > 118) y = 118;    //Out of bounds detection
        if(y < 0) y = 0;

        Entity->x = x;
        Entity->y = y;
        PLAYERUPDATE = 1;
    } else {
        PLAYERUPDATE = 0;
    }
    coordCounter = 0;
}

void updateEnemyCoords(Entity_t *Entity){   //this will MAYBE be implemented later
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    spdX = Entity->spdX;
    spdY = Entity->spdY;
    if((spdX != 0) || (spdY != 0)){ //update detection
        xOld = Entity->x;
        yOld = Entity->y;

        x = (xOld + spdX);
        y = (yOld + spdY);

        Entity->x = x;
        Entity->y = y;
        ENEMYUPDATE = 1;
    } else {
        ENEMYUPDATE = 0;
    }
}

//sets trajectory of bullet dependent on player quadrant location
void setPlayerBulletTrajectory(Entity_t *thePlayer, Entity_t *playerBullet){
    if(bulletLive != 2){
        if(thePlayer->x < 100 && thePlayer->x > 76) playerBullet->spdX = 0;
        else if(thePlayer->x > 100) playerBullet->spdX = -7;
        else if(thePlayer->x < 74) playerBullet->spdX = 7;

        if(thePlayer->y > 48 && thePlayer->y < 68) playerBullet->spdY = 0;
        else if(thePlayer->y > 48) playerBullet->spdY = -7;
        else if(thePlayer->y < 68) playerBullet->spdY = 7;
        bulletLive = 2;
    }
}

void updatePlayerBulletCoords(Entity_t *Bullet, Entity_t *thePlayer){
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    if(bulletLive == 0){                //bullet originates from player
        Bullet->x = thePlayer->x - 5;
        Bullet->y = thePlayer->y + 5;
        bulletHit = 0;
    } else if(bulletLive == 2) {        //bullet will go
        spdX = Bullet->spdX;
        spdY = Bullet->spdY;

        xOld = Bullet->x;
        yOld = Bullet->y;

        x = (xOld + spdX);
        y = (yOld + spdY);

        if((x < 93 && x > 68) && (y < 76 && y > 51)){   //bullet hit enemy?
            bulletHit = 1;
            bulletLive = 0;
            lastClear = 1;
            score++;
        } else if((x > 160 || x < 0) || (y > 128 || y < 0)){    //bullet hit wall?
            bulletLive = 0;
            bulletHit = 0;
            lastClear = 1;
        } else {
            Bullet->x = x;      //bullet keep go
            Bullet->y = y;
            bulletHit = 0;
        }
    }
}

void updateEnemyHP(Entity_t *Enemy){    //if bullet hit update hp--
    Enemy->hp = Enemy->hp - 1;
    if(Enemy->hp == 0) WIN = 1;
}

void updateEnemyBulletCoords(Entity_t *Entity){ //WIP
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    spdX = Entity->spdX;
    spdY = Entity->spdY;
    if((spdX != 0) || (spdY != 0)){ //update detection
        xOld = Entity->x;
        yOld = Entity->y;

        x = (xOld + spdX);
        if(x > 160) x = 160;    //Out of bounds detection (this will need to delete the bullet eventually)
        if(x < 4) x = 4;

        y = (yOld + spdY);
        if(y > 123) y = 123;    //Out of bounds detection (this will need to delete the bullet eventually)
        if(y < 0) y = 0;

        Entity->x = x;
        Entity->y = y;
    }
}


uint8_t SwitchHandler(uint32_t A, uint32_t B, Entity_t *thePlayer){ //Handles switch presses for player
    int8_t spdX, spdY;
    int16_t x, y;
    spdX = thePlayer->spdX;
    spdY = thePlayer->spdY;
    x = thePlayer->x;
    y = thePlayer->y;

  if((A&RT) == RT){ //shoots bullet if avail
      if(bulletLive == 0){
          bulletLive = 1;
      }
      return 1;
  }
  if((A&DWN) == DWN){   //Speed down
      spdX = spdX >> 1;
      if(spdX < 0) spdX++;
      spdY = spdY >> 1;
      if(spdY < 0) spdY++;
      thePlayer->spdX = spdX;
      thePlayer->spdY = spdY;
      return 2;
  }
  if((A&LFT) == LFT){   //will warp if warp avail and in bounds
    if(WARP){
        x += (spdX<<3);
        if(x > 180) x = 180;
        if(x < 0) x = 0;

        y += (spdY<<3);
        if(y > 128) y = 128;
        if(y < 0) y = 0;

        thePlayer->x = x;
        thePlayer->y = y;
        WARP = 0;
    }
    return 3;
  }
  if((A&UP) == UP){ //Will pause game
    //Pause
      return 4;
  }
  if((B&SP) == SP){ //Joystick button, tbd
    //TBD
      return 5;
  }
  return 0;
}


void setHPLED(Entity_t *thePlayer) {    //Sets HP LEDS based off player health
  HPFLAG = 0;
  if (thePlayer->hp >= 3)
  {
    LED_On(R3D | YEL | GRN);
  }
  else if (thePlayer->hp == 2)
  {
    LED_On(R3D | YEL);
    LED_Off(GRN);
  }
  else if (thePlayer->hp == 1)
  {
    LED_On(R3D);
    LED_Off(GRN | YEL);
  }
  else if (thePlayer->hp == 0)
  {
    LED_Off(GRN | YEL | R3D);
    GAMEOVER = 1;               //gameover :(
  }
}


uint8_t PxOld = 0, PyOld = 0;
void drawPlayer(Entity_t *thePlayer){   //draw player
    ST7735_DrawBitmap(PyOld, PxOld, playerOld, thePlayer->h, thePlayer->w);
    ST7735_DrawBitmap(thePlayer->y, thePlayer->x, player, thePlayer->h, thePlayer->w);
    PxOld = thePlayer->x;
    PyOld = thePlayer->y;
}

uint8_t ExOld, EyOld;
void drawEnemy(Entity_t *theEnemy){     //draw enemy
    ST7735_DrawBitmap(theEnemy->y, theEnemy->x, enemy, theEnemy->h, theEnemy->w);
    ENEMYUPDATE = 0;
}

uint8_t PBxOld = 0, PByOld = 0;
void drawPlayerBullet(Entity_t *Bullet){        //draw bullet
    ST7735_DrawBitmap(PByOld, PBxOld, player_BulletOld, 2, 2);
    ST7735_DrawBitmap(Bullet->y, Bullet->x, player_Bullet, 2, 2);
    PBxOld = Bullet->x;
    PByOld = Bullet->y;
}

void clearPlayerBullet(){       //clears last instance of bullet
    ST7735_DrawBitmap(PByOld, PBxOld, player_BulletOld, 2, 2);
    lastClear = 0;
}


void drawLangScrn(){        //Language select screen
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Language", 2, ST7735_WHITE);
    ST7735_SetCursor(11, 5);
    ST7735_OutStringCool("English", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 7);
    ST7735_OutStringCool("Bosanski / Hrvatski", 1, ST7735_WHITE);
    ST7735_SetCursor(9, 10);
    ST7735_OutStringCool("Jezik", 2, ST7735_WHITE);

    ST7735_DrawBitmap(47, 111, iconup, 11, 11);
    ST7735_DrawBitmap(68, 147, icondwn, 11, 11);
}

void drawEngOpt(){
    ST7735_SetCursor(7, 1);
    ST7735_OutStringCool("Options", 2, ST7735_WHITE);
    ST7735_SetCursor(4, 3);
    ST7735_OutStringCool("       PlyrHP BossHP", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 4);
    ST7735_OutStringCool("Demo     13    min", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 5);
    ST7735_OutStringCool("Easy      7    low", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 6);
    ST7735_OutStringCool("Normal    5    med", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 7);
    ST7735_OutStringCool("Hard      3    high", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 9);
    ST7735_OutStringCool("No-Hit    1    most", 1, ST7735_WHITE);

    ST7735_DrawBitmap(35, 155, iconup, 11, 11);
    ST7735_DrawBitmap(47, 155, iconrt, 11, 11);
    ST7735_DrawBitmap(59, 155, icondwn, 11, 11);
    ST7735_DrawBitmap(71, 155, iconlft, 11, 11);
    ST7735_DrawBitmap(89, 155, joystick, 10, 10);
}

void drawBHOpt(){
    ST7735_SetCursor(7, 1);
        ST7735_OutStringCool("Opcije", 2, ST7735_WHITE);
        ST7735_SetCursor(4, 3);
        ST7735_OutStringCool("       IgrcHP SefHP", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 4);
        ST7735_OutStringCool("Demo     13    manje", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 5);
        ST7735_OutStringCool("Lako      7    malo", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 6);
        ST7735_OutStringCool("Normalno  5   srednje", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 7);
        ST7735_OutStringCool("Tesko     3    puno", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 9);
        ST7735_OutStringCool("Bez-Udar  1    vise", 1, ST7735_WHITE);

        ST7735_DrawBitmap(35, 155, iconup, 11, 11);
        ST7735_DrawBitmap(47, 155, iconrt, 11, 11);
        ST7735_DrawBitmap(59, 155, icondwn, 11, 11);
        ST7735_DrawBitmap(71, 155, iconlft, 11, 11);
        ST7735_DrawBitmap(89, 155, joystick, 10, 10);
}

void drawTitleEng(){
    ST7735_DrawBitmap(13, 143, TITLE, 15, 127);
    ST7735_SetCursor(11, 4);
    ST7735_OutStringCool("Start", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 6);
    ST7735_OutStringCool("Options", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 8);
    ST7735_OutStringCool("Controls", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 10);
    ST7735_OutStringCool("Lore", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 111, iconup, 11, 11);
    ST7735_DrawBitmap(57, 111, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 111, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 111, iconrt, 11, 11);

}

void drawTitleBH(){
    ST7735_DrawBitmap(13, 143, TITLE, 15, 127);
    ST7735_SetCursor(11, 4);
    ST7735_OutStringCool("Pocni", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 6);
    ST7735_OutStringCool("Opcije", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 8);
    ST7735_OutStringCool("Kontrole", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 10);
    ST7735_OutStringCool("Prica", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 111, iconup, 11, 11);
    ST7735_DrawBitmap(57, 111, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 111, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 111, iconrt, 11, 11);
}

void gameOverEng(){
    ST7735_SetCursor(4, 1);
    ST7735_OutStringCool("GAME OVER!", 2, ST7735_RED);
    ST7735_SetCursor(10, 7);
    ST7735_OutStringCool("Score:", 1, ST7735_WHITE);
    printf("%i", score);

}

void gameOverBH(){
    ST7735_SetCursor(2, 1);
    ST7735_OutStringCool("IGRA GOTOVA!", 2, ST7735_RED);
    ST7735_SetCursor(9, 7);
    ST7735_OutStringCool("Rezultat:", 1, ST7735_WHITE);
    printf("%i", score);
}

void winEng(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("YOU WIN!", 2, ST7735_GREEN);
    ST7735_SetCursor(10, 7);
    ST7735_OutStringCool("Score:", 1, ST7735_WHITE);
    printf("%i", score);
}

void winBH(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("POBJEDA!", 2, ST7735_GREEN);
    ST7735_SetCursor(9, 7);
    ST7735_OutStringCool("Rezultat:", 1, ST7735_WHITE);
    printf("%i", score);
}

void controlsEng(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Controls", 2, ST7735_WHITE);
    ST7735_SetCursor(5, 4);
    ST7735_OutStringCool("Shoot - Auto aim", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 6);
    ST7735_OutStringCool("Speed shift - Slow", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 8);
    ST7735_OutStringCool("Warp - Teleport ~7sec", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 10);
    ST7735_OutStringCool("Pause - Pause game", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 151, iconrt, 11, 11);
    ST7735_DrawBitmap(57, 151, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 151, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 151, iconup, 11, 11);
}

void controlsBH(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Kontrole", 2, ST7735_WHITE);
    ST7735_SetCursor(5, 4);
    ST7735_OutStringCool("Pucaj - Auto aim", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 6);
    ST7735_OutStringCool("Mjen brzine - Sporije", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 8);
    ST7735_OutStringCool("Warp - Teleport ~7sec", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 10);
    ST7735_OutStringCool("Pauza - Zaustavi igru", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 151, iconrt, 11, 11);
    ST7735_DrawBitmap(57, 151, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 151, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 151, iconup, 11, 11);
}

void loreEng(){
    ST7735_SetCursor(1, 0);
    ST7735_OutStringCool("Word has it that Valvano ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 1);
    ST7735_OutStringCool("will make the final impos-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 2);
    ST7735_OutStringCool("-sible. He has fled in his", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("spaceship to avoid being", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("hunted down. You have ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("decided to hunt him. Beat", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("him to get the answer key", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("and save the 319k students", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool("  You are the last hope.  ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 11);
    ST7735_OutStringCool("    Press any button", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 12);
    ST7735_OutStringCool("       to return", 1, ST7735_WHITE);
}

void loreBH(){
    ST7735_SetCursor(1, 0);
    ST7735_OutStringCool("Izgleda da ce Valvano", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 1);
    ST7735_OutStringCool("napravit zadnji ispit nem-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 2);
    ST7735_OutStringCool("-oguce. Pobjego je u svem-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("-irski brod da ga ne bi", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("lovili studente. Ti ces", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("ga uhvatit. Pobjedi ga da", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("dobjes odgovore za ispit,", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("i da spasis 319k studente", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool(" Ti si zadnji sto mozes. ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 11);
    ST7735_OutStringCool("    Pritizni sta god", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 12);
    ST7735_OutStringCool("      da se vratis", 1, ST7735_WHITE);
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

void langSelect(){
    drawLangScrn();
    if((switchDataA&UP) == UP){
        LANGMODE = 1; //ENG
        LANGSELECT = 0;
        MAINMENU = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){
        LANGMODE = 2; //BH
        LANGSELECT = 0;
        MAINMENU = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void mainMenu(){
    if(LANGMODE == 1) drawTitleEng();
    if(LANGMODE == 2) drawTitleBH();

    if((switchDataA&UP) == UP){
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
    if((switchDataA&RT) == RT){
        LORE = 1;
        MAINMENU = 0;
        LOREUPDATE = 1;
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void win(){
    if(LANGMODE == 1) winEng();
    if(LANGMODE == 2) winBH();
}

void lose(){
    if(LANGMODE == 1) gameOverEng();
    if(LANGMODE == 2) gameOverBH();
}

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

void Options(Entity_t *thePlayer, Entity_t *theEnemy){  //Sets difficulty (playerHP and enemyHP)
    if(LANGMODE == 1) drawEngOpt();
    if(LANGMODE == 2) drawBHOpt();

    if((switchDataA&UP) == UP){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPdemo);             // inits player
        enemyInit(theEnemy, enemyHPdemo);               // inits enemy
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&RT) == RT){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPeasy);             // inits player
        enemyInit(theEnemy, enemyHPeasy);               // inits enemy
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&DWN) == DWN){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPnormal);             // inits player
        enemyInit(theEnemy, enemyHPnormal);               // inits enemy
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataA&LFT) == LFT){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPhard);             // inits player
        enemyInit(theEnemy, enemyHPhard);               // inits enemy
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
    if((switchDataB&SP) == SP){
        OPTIONSELECT = 0;
        MAINMENU = 1;
        playerInit(thePlayer, playerHPnohit);             // inits player
        enemyInit(theEnemy, enemyHPnohit);               // inits enemy
        ST7735_FillScreen(ST7735_BLACK);
        Clock_Delay1ms(250);
    }
}

void drawEnemyBullet(){

}

uint8_t COLLISIONFLAG;
void collisionCheck(Entity_t *Player, Entity_t *Entity) {   //check collision
    if(Entity->w == 25) crashCheck(Player, Entity);
    else bulletCollisionCheck(Player, Entity);
}

void crashCheck(Entity_t *Player, Entity_t *Enemy) {    //check if crashed
    if((Player->x < (Enemy->x + 10) && Player->x > (Enemy->x - 23)) && (Player->y < (Enemy->y + 23) && Player->y > (Enemy->y - 10))) { //Collision with enemy detection
        HPFLAG = 1;
        COLLISIONFLAG = 1;
        Player->hp = 0;
        CRASH = 1;
    }
}

void bulletCollisionCheck(Entity_t *Player, Entity_t *Bullet) { //WIP
    if((Player->x < (Bullet->x) && Player->x > (Bullet->x)) && (Player->y < (Bullet->y) && Player->y > (Bullet->y))) { //Collision with enemy detection
        HPFLAG = 1;
        COLLISIONFLAG = 1;
        Player->hp = Player->hp - 1;
    }
}

/*
void newPLayerBullet(*PlayetBullet_t a, uint_8_t x, uint8_t y, ... (everything else)){

    a->x = x;
    a->y = y;
    }
 */


/*
TIMER_IRQHandler3(){ //Variable rate
    Depending on boss phase and attack type,
    Execute different bullet patterns
}
*/

/*
uint8_t PHASE = 0;
Phase_Switcher(Enemy_t Enemy){
    //Phase 0 is intro phase, easy stuff
    if(Enemy.hp < 75%maxHP){
      PHASE = 1;    //Phase 1 is a bit harder, more bullets at once
    }
    if(Enemy.hp < 50%maxHP){
      PHASE = 2;    //Phase 2: even more bullets and pattern
    }
    if(Enemy.hp < 25%maxHP){
      PHASE = 3;    //Phase 3: Brutha starts moving arround and more bullets?
    }
    if(Enemy.hp < 5%maxHP){
      PHASE = 4;    //Phase 4: Desperation
    }
    if(Enemy.hp < 1%maxHP){
      PHASE = 5;    //Phase 5: Combustion phase
    }
}
*/

/*
Phase_Handler(){    Need to figure out how this would actually be implemented, FSM maybe?
  if(PHASE == 0){
    Shoot Bullets slowly at player
  }
  if(PHASE == 1){
    Shoot bullets a bit faster, and add a spray
  }
  if(PHASE == 2){
    //TBD
  }
  if(PHASE == 3){
    //TBD
  }
  if(PHASE == 4){
    Goto center and start spamming
  }
  if(PHASE == 5){
    Goto center and just explode in a bunch of bullets
  }
}


*/

/*
PauseMenu(){  When paused, go here until unpause
  Options:
  Resume
  Quit
}
*/

/*
Gamemenu(){ //Start herer
    Set gamemode -> Set HP values
    Choose language
    Start Game

    Difficulties:     Enemy Base HP calculation: Bullets do 1 damage, can be shot 4 times a second. 1 minute of sustained damage should be base. Base = 240HP
    Easy - 7hp | Enemy - Base/1.5 = 160hp (45sec)
    Normal - 5hp | Enemy - Base = 240hp (1min)
    Hard - 3hp | Enemy - Base*1.25  = 300hp (1:15)
    No-Hit - 1hp | Enemy - Base*1.5 = 360hp (1:30)
}
*/
