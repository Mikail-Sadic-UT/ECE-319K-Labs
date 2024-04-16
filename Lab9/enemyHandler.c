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
#include "enemyHandler.h"

uint8_t bulletCounter1, bulletCounter2, bulletCounter3;
uint8_t bulletTimer1, bulletTimer2, bulletTimer3;
uint8_t phase0, phase1, phase2, phase3, phase4, phase5;
uint8_t activate1, activate2, activate3;
uint8_t pattern1, pattern2, pattern3;
uint8_t PHASE;

void phaseTimer(){
    bulletCounter1++;
    bulletCounter2++;
    bulletCounter3++;
    if((bulletCounter1 > bulletTimer1) && activate1){
        pattern1 = 1;
        bulletCounter1 = 0;
    }
    if((bulletCounter2 > bulletTimer2) && activate2){
        pattern2 = 1;
        bulletCounter2 = 0;
    }
    if((bulletCounter3 > bulletTimer3) && activate3){
        pattern3 = 1;
        bulletCounter3 = 0;
    }
}

void phaseInit(uint8_t hp){
    phase0 = hp - hp/10;    //90%
    phase1 = hp/2 + hp/4;    //75%
    phase2 = hp/2;    //50%
    phase3 = hp/4;    //25%
    phase4 = hp/10;    //10%
    phase5 = 0;    //0%
}


void Phase_Switcher(Entity_t *Enemy){
    //Phase 0 is intro phase, shoots bullets at player slowley
    if(Enemy->hp < phase4){
        PHASE = 5;    //Phase 5: Combustion phase (as many bullets as can be on screen at once)
    }
    else if(Enemy->hp < phase3){
        PHASE = 4;    //Phase 4: Desperation
    }
    else if(Enemy->hp < phase2){
        PHASE = 3;    //Phase 3: More bullets
    }
    else if(Enemy->hp < phase1){
        PHASE = 2;    //Phase 2: even more bullets and pattern
    }
    else if(Enemy->hp < phase0){
        PHASE = 1;    //Phase 1 is a bit harder, shoots faster and sprays
    }
}

void Phase_Handler(){
    if(PHASE == 0){
        activate1 = 1;
        activate2 = 0;
        activate3 = 0;
        bulletTimer1 = 30;
    }
    else if(PHASE == 1){
        activate1 = 1;
        activate2 = 1;
        activate3 = 0;
        bulletTimer1 = 25;
        bulletTimer2 = 120;
    }
    else if(PHASE == 2){
        activate1 = 1;
        activate2 = 1;
        activate3 = 0;
        bulletTimer1 = 20;
        bulletTimer2 = 90;
    }
    else if(PHASE == 3){
        activate1 = 1;
        activate2 = 1;
        activate3 = 1;
        bulletTimer1 = 20;
        bulletTimer2 = 90;
        bulletTimer3 = 120;
    }
    else if(PHASE == 4){
        activate1 = 1;
        activate2 = 1;
        activate3 = 1;
        bulletTimer1 = 15;
        bulletTimer2 = 60;
        bulletTimer3 = 90;
    }
    else if(PHASE == 5){
        activate1 = 0;
        activate2 = 0;
        activate3 = 1;
        bulletTimer3 = 30;
    }
}

void Pattern_Executer(){
    if(pattern1){
        //shoot 1 bullet at player
        pattern1 = 0;
    }
    if(pattern2){
        //shoot burst at player
        pattern2 = 0;
    }
    if(pattern3){
        //shoot in circle
        pattern3 = 0;
    }
}

/*
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
        dx = dx<<3;
        dy = dy<<3;
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
            bulletInit(thePlayer, Bullet);
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
