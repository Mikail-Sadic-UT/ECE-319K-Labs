#ifndef ENEMYHANDLER_H_
#define ENEMYHANDLER_H_

#include <stdint.h>
#include "Engine.h"

void phaseTimer();

void phaseInit(uint8_t hp);

void Phase_Switcher(Entity_t *Enemy);

void Phase_Handler();

#endif

