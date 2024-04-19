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
extern uint8_t HPFLAG;

#define enemyBulletBuffer 64
Entity_t enemyBullets[enemyBulletBuffer];

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
        bulletTimer2 = 90;
    }
    else if(PHASE == 2){
        activate1 = 1;
        activate2 = 1;
        activate3 = 0;
        bulletTimer1 = 20;
        bulletTimer2 = 60;
    }
    else if(PHASE == 3){
        activate1 = 1;
        activate2 = 1;
        activate3 = 1;
        bulletTimer1 = 15;
        bulletTimer2 = 60;
        bulletTimer3 = 120;
    }
    else if(PHASE == 4){
        activate1 = 1;
        activate2 = 1;
        activate3 = 1;
        bulletTimer1 = 10;
        bulletTimer2 = 45;
        bulletTimer3 = 90;
    }
    else if(PHASE == 5){
        activate1 = 0;
        activate2 = 0;
        activate3 = 1;
        bulletTimer3 = 30;
    }
}

uint8_t curBullet;

void Pattern_Executer(Entity_t *thePlayer, Entity_t *theEnemy){
    uint8_t x1, x2, x3;
    if(PHASE == 0) x1 = 2;
    else if(PHASE == 1) x1 = 2, x2 = 1;
    else if(PHASE == 2) x1 = 3, x2 = 2;
    else if(PHASE == 3) x1 = 3, x2 = 2;
    else if(PHASE == 4) x1 = 3, x2 = 2;

    if(pattern1){ //shoot 1 bullet at player
        pattern1 = 0;
        curBullet++;
        setEnemyBulletTrajectory(thePlayer, theEnemy, curBullet, 0, 0, x1);
        if(curBullet > enemyBulletBuffer) curBullet = 0;
    }
    if(pattern2){ //shoot burst at player
        pattern2 = 0;
        curBullet++;
        setEnemyBulletTrajectory(thePlayer, theEnemy, curBullet, -2, 2, x2);
        curBullet++;
        setEnemyBulletTrajectory(thePlayer, theEnemy, curBullet, 0, 0, x2);       //this is hopefully like a spray
        curBullet++;
        setEnemyBulletTrajectory(thePlayer, theEnemy, curBullet, 2, -2, x2);
        if(curBullet > enemyBulletBuffer) curBullet = 0;
    }
    if(pattern3){
        circleTrajectory();
        pattern3 = 0;
    }
}

void circleTrajectory(){
    int8_t y = -4;

    for(int8_t x = 0; x < 4; x++){
        curBullet++;
        if(curBullet > enemyBulletBuffer) curBullet = 0;
        enemyBullets[curBullet].live = 2;
        enemyBullets[curBullet].spdX = x;
        enemyBullets[curBullet].spdY = y;
        y++;
    }

    y = -4;
    for(int8_t x = 0; x < 4; x++){
        curBullet++;
        if(curBullet > enemyBulletBuffer) curBullet = 0;
        if(x != 0){
            enemyBullets[curBullet].live = 2;
            enemyBullets[curBullet].spdX = (x*-1);
            enemyBullets[curBullet].spdY = (y*-1);
        }
        y++;
    }

    y = 4;
    for(int8_t x = 0; x < 4; x++){
        curBullet++;
        if(curBullet > enemyBulletBuffer) curBullet = 0;
        enemyBullets[curBullet].live = 2;
        enemyBullets[curBullet].spdX = x;
        enemyBullets[curBullet].spdY = y;
        y--;
    }

    y = 4;
    for(int8_t x = 0; x < 4; x++){
        curBullet++;
        if(curBullet > enemyBulletBuffer) curBullet = 0;
        enemyBullets[curBullet].live = 2;
        enemyBullets[curBullet].spdX = (y*-1);
        enemyBullets[curBullet].spdY = (x*-1);
        y--;
    }

    curBullet++;
    if(curBullet > enemyBulletBuffer) curBullet = 0;
    enemyBullets[curBullet].live = 2;
    enemyBullets[curBullet].spdX = 4;
    enemyBullets[curBullet].spdY = 0;

}

//Sets bullet trajectory to enemy (it just works)
void setEnemyBulletTrajectory(Entity_t *thePlayer, Entity_t *theEnemy, uint8_t i, int8_t Xoff, int8_t Yoff, uint8_t x){
    enemyBullets[i].live++;
    if(enemyBullets[i].live == 1){
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
        dx = dx<<x;
        dy = dy<<x;
        spdX = dx/b;
        spdY = dy/b;


        spdX = spdX*-1; //x inversion

        enemyBullets[i].spdX = spdX + Xoff;
        enemyBullets[i].spdY = spdY + Yoff;

        enemyBullets[i].live = 2;
    }
}



void updateEnemyBulletCoords(Entity_t *thePlayer, Entity_t *theEnemy, uint8_t i){
    int8_t spdX, spdY;
    int16_t xOld, yOld, x, y;
    if(enemyBullets[i].live == 0){                //bullet originates from enemy
        enemyBullets[i].x = theEnemy->x - 10;
        enemyBullets[i].y = theEnemy->y + 10;
    } else if(enemyBullets[i].live >= 1) {        //bullet will go
        spdX = enemyBullets[i].spdX;
        spdY = enemyBullets[i].spdY;

        xOld = enemyBullets[i].x;
        yOld = enemyBullets[i].y;

        x = (xOld + spdX);
        y = (yOld + spdY);

        if((x < (thePlayer->x + 3) && x > (thePlayer->x - 9)) && (y < (thePlayer->y + 9) && y > (thePlayer->y - 3))){
            HPFLAG = 1;
            thePlayer->hp = thePlayer->hp - 1;
            enemyBullets[i].xHit = enemyBullets[i].x;
            enemyBullets[i].yHit = enemyBullets[i].y;
            bulletReset(theEnemy, i);
            enemyBullets[i].lastClear = 1;
        } else if((x > 160 || x < 0) || (y > 128 || y < 0)){    //bullet hit wall?
            enemyBullets[i].xHit = enemyBullets[i].x;
            enemyBullets[i].yHit = enemyBullets[i].y;
            bulletReset(theEnemy, i);
            enemyBullets[i].lastClear = 1;
        } else {
            enemyBullets[i].x = x;      //bullet keep go
            enemyBullets[i].y = y;
        }
    }
}

uint8_t type = 1;
void bulletReset(Entity_t *theEnemy, uint8_t i){
    enemyBullets[i].x = theEnemy->x - 10;
    enemyBullets[i].y = theEnemy->y + 10;
    enemyBullets[i].xOld = theEnemy->x - 10;
    enemyBullets[i].yOld = theEnemy->y + 10;
    enemyBullets[i].live = 0;
    enemyBullets[i].lastClear = 0;
    type++;
    if(type > 2) type = 1;
    enemyBullets[i].type = type;

}
