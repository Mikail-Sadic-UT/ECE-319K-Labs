#ifndef ENEMYHANDLER_H_
#define ENEMYHANDLER_H_

#include <stdint.h>
#include "Engine.h"

void phaseTimer();

void phaseInit(uint8_t hp);

void Phase_Switcher(Entity_t *Enemy);

void Phase_Handler();

void setEnemyBulletTrajectory(Entity_t *thePlayer, Entity_t *theEnemy, uint8_t i, int8_t Xoff, int8_t Yoff, uint8_t x);

void Pattern_Executer(Entity_t *thePlayer, Entity_t *theEnemy);

void bulletReset(Entity_t *theEnemy, uint8_t i);

void updateEnemyBulletCoords(Entity_t *thePlayer, Entity_t *theEnemy, uint8_t i);

void circleTrajectory();

#endif

