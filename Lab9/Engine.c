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
#include "Lab9Main.h"
#include "menuHandler.h"

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
extern uint8_t GAMEOVER;
extern uint8_t CRASH;
extern uint8_t bulletHit;
extern uint8_t lastClear;
extern uint8_t WIN;
extern uint8_t GAMESTART;
extern uint8_t LANGMODE;
extern uint32_t switchDataA;
extern uint32_t switchDataB;
extern uint8_t score;
extern uint8_t PAUSED;
uint32_t collisioncheck = 0;
uint8_t CONTINUE;

Entity_t warpLocation;

#define HIT 1
#define HITP 3000
#define Crash 2
#define CRASHP 5000
#define Warp 3
#define WARPP 5000
#define Expl 4
#define EXPLP 5000

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

void bulletInit(Entity_t *thePlayer, Entity_t *Bullet){ //Initializes bullet
    Bullet->x = thePlayer->x - 6;
    Bullet->y = thePlayer->y + 6;
    Bullet->live = 0;
}

void updateCoords(Entity_t *Entity){    //Updates coord of entity
    if(Entity->w == 11) updatePlayerCoords(Entity);
    if(Entity->w == 25) updateEnemyCoords(Entity);
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
}

void updateIndicatorCoords(Entity_t *thePlayer){
    int16_t x, y;
    x = (thePlayer->x - 3) + (thePlayer->spdX<<5);
    if(x > 159) x = 159;
    if(x < 5) x = 5;

    y = (thePlayer->y + 3) + (thePlayer->spdY<<5);
    if(y > 123) y = 123;
    if(y < 0) y = 0;

    warpLocation.x = x;
    warpLocation.y = y;
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

//Sets bullet trajectory to enemy (it just works)
void setPlayerBulletTrajectory(Entity_t *thePlayer, Entity_t *playerBullet, Entity_t *theEnemy){
    playerBullet->live++;
    if(playerBullet->live == 1){
        uint8_t Px = thePlayer->x - 6;
        uint8_t Py = thePlayer->y + 6;
        uint8_t Ex = theEnemy->x - 13;
        uint8_t Ey = theEnemy->y + 13;
        int16_t dx, dy, n1, n2, b;
        int8_t spdX, spdY;

        dx = Ex - Px;
        dy = Py - Ey;
        n1 = dx;
        if(n1 < 0) n1*=-1;
        n2 = dy;
        if(n2 < 0) n2*=-1;
        b = n1 + n2;
        dx = (dx<<3) + (dx<<1);
        dy = (dy<<3) + (dy<<1);
        spdX = dx/b;
        spdY = dy/b;
        spdY = spdY*-1;

        playerBullet->spdX = spdX;
        playerBullet->spdY = spdY;

        playerBullet->live = 2;
    }
}

void updatePlayerBulletCoords(Entity_t *Bullet, Entity_t *thePlayer, Entity_t *theEnemy){
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    if(Bullet->live == 0){                //bullet originates from player
        Bullet->x = thePlayer->x - 5;
        Bullet->y = thePlayer->y + 5;
        bulletHit = 0;
    } else if(Bullet->live >= 1) {        //bullet will go
        spdX = Bullet->spdX;
        spdY = Bullet->spdY;

        xOld = Bullet->x;
        yOld = Bullet->y;

        x = (xOld + spdX);
        y = (yOld + spdY);

        if((x < (theEnemy->x) && x > (theEnemy->x - 23)) && (y < (theEnemy->y + 23) && y > (theEnemy->y))){
            bulletHit = 1;
            bulletInit(thePlayer, Bullet);          //bullet hit enemy?
            lastClear = 1;
            score++;
        } else if((x > 160 || x < 0) || (y > 128 || y < 0)){    //bullet hit wall?
            bulletInit(thePlayer, Bullet);
            bulletHit = 0;
            lastClear = 1;
        } else {
            Bullet->x = x;      //bullet keep go
            Bullet->y = y;
            bulletHit = 0;
        }
    }
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


//Handles switch presses for player
uint8_t SwitchHandler(uint32_t A, uint32_t B, Entity_t *thePlayer, Entity_t *theBullet, Entity_t *theEnemy){
    int8_t spdX, spdY;
    int16_t x, y;
    spdX = thePlayer->spdX;
    spdY = thePlayer->spdY;
    x = thePlayer->x;
    y = thePlayer->y;

  if((A&RT) == RT){ //shoots bullet if avail
      return 1;
  }
  if((A&DWN) == DWN){   //Speed up
      spdX = spdX << 1;
      if(spdX < 0) spdX++;
      spdY = spdY << 1;
      if(spdY < 0) spdY++;
      thePlayer->spdX = spdX;
      thePlayer->spdY = spdY;
      return 2;
  }
  if((A&LFT) == LFT){   //will warp if warp avail and in bounds
    if(WARP){
        Sound_Start(WARPP, Warp); //play warp sound

        x += (spdX<<5);
        if(x > 180) x = 180;
        if(x < 0) x = 0;

        y += (spdY<<5);
        if(y > 128) y = 128;
        if(y < 0) y = 0;

        thePlayer->x = x;
        thePlayer->y = y;
        WARP = 0;
    }
    return 3;
  }
  if((A&UP) == UP){ //Will pause game
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
  if (thePlayer->hp >= 3) {
    LED_On(R3D | YEL | GRN);
  }
  else if (thePlayer->hp == 2) {
    LED_On(R3D | YEL);
    LED_Off(GRN);
  }
  else if (thePlayer->hp == 1) {
    LED_On(R3D);
    LED_Off(GRN | YEL);
  }
  else if (thePlayer->hp <= 0) {
    LED_Off(GRN | YEL | R3D);
    GAMESTART = 0;
    GAMEOVER = 1;               //gameover :(
  }
}

void updateEnemyHP(Entity_t *Enemy){    //if bullet hit update hp--
    Enemy->hp = Enemy->hp - 1;
    if(Enemy->hp <= 0) {
        GAMESTART = 0;
        WIN = 1;
    }
}

void winHandler(Entity_t *theEnemy){    //Handles win stuff
    drawEnemyDeath(theEnemy);
    Sound_Start(EXPLP, Expl);
    Clock_Delay1ms(250);
    win();
    Clock_Delay1ms(500);
    while(WIN){
        if(switchDataA > 0) {
            answers();
            while(WIN){
                if(switchDataA > 0){
                    Clock_Delay1ms(250);
                    reset();
                    Clock_Delay1ms(100);
                }
            }
        }
    }
}

void gameEndHandler(Entity_t *thePlayer){   //Handles not win stuff
    drawPlayerDeath(thePlayer);
    if(CRASH){
        Sound_Start(CRASHP, Crash);
        ST7735_SetCursor(8, 9);
        if(LANGMODE == 1) ST7735_OutStringCool("You crashed!", 1, ST7735_ORANGE);
        if(LANGMODE == 2) ST7735_OutStringCool("Sudarijo se!", 1, ST7735_ORANGE);
    } else Sound_Start(EXPLP, Expl);
    Clock_Delay1ms(250);
    ST7735_SetCursor(1, 11);
    if(LANGMODE == 1) ST7735_OutStringCool("Press any button to reset", 1, ST7735_WHITE);
    if(LANGMODE == 2) {
        ST7735_SetCursor(4, 11);
        ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 12);
        ST7735_OutStringCool("     resetujes     ", 1, ST7735_WHITE);
    }
    lose();
    Clock_Delay1ms(500);
    while(GAMEOVER){
        if(switchDataA > 0){
            Clock_Delay1ms(250);
            reset();
            Clock_Delay1ms(100);
        }
    }
}
