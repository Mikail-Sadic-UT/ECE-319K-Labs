#ifndef ENGINE_H_
#define ENGINE_H_

struct Entity
{
  int8_t hp;
  uint8_t x;
  uint8_t y;
  uint8_t h, w;
  int8_t spdX;
  int8_t spdY;
  uint8_t live; //for bullets if live
  uint8_t type;
  uint8_t xOld, yOld;
  uint8_t lastClear;
  uint8_t xHit, yHit;
};
typedef struct Entity Entity_t;


void playerInit(Entity_t *thePlayer, uint16_t hp);  //Initializes player

void enemyInit(Entity_t *theEnemy, uint16_t hp);    //Initializes enemy

void bulletInit(Entity_t *thePlayer, Entity_t *Bullet);

void setSpeed(Entity_t *thePlayer);     //sets Player speed

void updateCoords(Entity_t *thePlayer); //Updates coords of entity

void updatePlayerCoords(Entity_t *Entity);  //Updates player coords

void updateIndicatorCoords(Entity_t *thePlayer);

void updateEnemyCoords(Entity_t *Entity);   //Updates enemy coords (WIP)

void setPlayerBulletTrajectory(Entity_t *thePlayer, Entity_t *playerBullet, Entity_t *theEnemy);

void updatePlayerBulletCoords(Entity_t *Bullet, Entity_t *thePlayer, Entity_t *theEnemy);    //WIP

void setHPLED(Entity_t *thePlayer);     //Sets HP LEDS based off player health

void collisionCheck(Entity_t *Player, Entity_t *Enemy); //Checks collisions

void crashCheck(Entity_t *Player, Entity_t *Enemy); //ends game if crash into enemy

void bulletCollisionCheck(Entity_t *Player, Entity_t *Bullet);  //WIP

uint8_t SwitchHandler(uint32_t A, uint32_t B, Entity_t *thePlayer, Entity_t *theBullet, Entity_t *theEnemy); //Handles switch presses for player

void updateEnemyHP(Entity_t *Enemy);    //updates enemy hp on hit

void winHandler(Entity_t *theEnemy);

void gameEndHandler(Entity_t *thePlayer);

#endif
