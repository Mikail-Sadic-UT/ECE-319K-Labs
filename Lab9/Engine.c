#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
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

#define R3D (1 << 17)
#define YEL (1 << 28) // Info
#define GRN (1 << 31)

#define UP (1<<24)
#define DWN (1<<26)
#define LFT (1<<25)
#define RT (1<<27)
#define SP (1<<16)

extern uint32_t *ADCX;
extern uint32_t *ADCY;
extern uint8_t HPFLAG;
extern uint8_t DASH;
extern uint8_t PLAYERUPDATE;

void playerInit(Player_t *thePlayer){
    thePlayer->x = 90;
    thePlayer->y = 64;
}

void setSpeed(Player_t *thePlayer)
{
  int32_t X = ADCX;
  int32_t Y = ADCY;
  X = X >> 9;
  Y = Y >> 9;

    X = X - 4; // X speed is -7 to +7
    if(X < 0) X++;

    Y = Y - 4; // Y speed is -7 to +7
    if(Y < 0) Y++;

    Y = Y*-1;
    thePlayer->spdX = X;
    thePlayer->spdY = Y;
}

void updateCoords(Player_t *thePlayer){
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    spdX = thePlayer->spdX;
    spdY = thePlayer->spdY;
    if((spdX != 0) || (spdY != 0)){
        xOld = thePlayer->x;
        yOld = thePlayer->y;

        x = (xOld + spdX);
        if(x > 160) x = 160;
        if(x < 10) x = 10;

        y = (yOld + spdY);
        if(y > 118) y = 118;
        if(y < 0) y = 0;

        thePlayer->x = x;
        thePlayer->y = y;
        PLAYERUPDATE = 1;
    } else {
        PLAYERUPDATE = 0;
    }
}

uint8_t SwitchHandler(uint32_t A, uint32_t B, Player_t *thePlayer){
    int8_t spdX, spdY;
    int16_t x, y;
    spdX = thePlayer->spdX;
    spdY = thePlayer->spdY;
    x = thePlayer->x;
    y = thePlayer->y;

  if((A&RT) == RT){
      /*    Available every .25second
      Make new bullet
      Set bullet trajectory to Enemy
      PlayerBullet_t a;
      newPlayerBullet(&a, 0, 0);
      */
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
  if((A&LFT) == LFT){
    if(DASH){
        x += (spdX<<3);
        if(x > 180) x = 180;
        if(x < 0) x = 0;

        y += (spdY<<3);
        if(y > 128) y = 128;
        if(y < 0) y = 0;

        thePlayer->x = x;
        thePlayer->y = y;
        DASH = 0;
    }
    return 3;
  }
  if((A&UP) == UP){
    //Pause
      return 4;
  }
  if((B&SP) == SP){ //Joystick button, tbd
    //TBD
      return 5;
  }
  return 0;
}


void setHPLED(Player_t *Player)
{
  HPFLAG = 0;
  if (Player->hp >= 3)
  {
    LED_On(R3D | YEL | GRN);
  }
  else if (Player->hp == 2)
  {
    LED_On(R3D | YEL);
    LED_Off(GRN);
  }
  else if (Player->hp == 1)
  {
    LED_On(R3D);
    LED_Off(GRN | YEL);
  }
  else if (Player->hp == 0)
  {
    LED_Off(GRN | YEL | RED);
  }
}

uint8_t COLLISIONFLAG;
uint8_t collisionCheck(Player_t *Player)
{
  if (Player->x || Player->y)
  { // If player has collided with bullet (need to implement)
    HPFLAG = 1;
    COLLISIONFLAG = 1;
    Player->hp = Player->hp - 1;
    return 1;
  }
  return 0;
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
