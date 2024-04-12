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

extern uint32_t *ADCX;
extern uint32_t *ADCY;
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
