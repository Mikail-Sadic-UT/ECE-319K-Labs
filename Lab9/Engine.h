#ifndef ENGINE_H_
#define ENGINE_H_

struct Entity
{
  uint8_t hp;
  uint8_t x;
  uint8_t y;
  uint32_t *image; // img of sprite
  uint8_t h, w;    // height and width of sprite
  int8_t spdX;
  int8_t spdY;
};
typedef struct Entity Entity_t;


void playerInit(Entity_t *thePlayer, uint16_t hp);  //Initializes player

void enemyInit(Entity_t *theEnemy, uint16_t hp);    //Initializes enemy

void setSpeed(Entity_t *thePlayer);     //sets Player speed

void updateCoords(Entity_t *thePlayer); //Updates coords of entity

void updatePlayerCoords(Entity_t *Entity);  //Updates player coords

void updateEnemyCoords(Entity_t *Entity);   //Updates enemy coords (WIP)

void setPlayerBulletTrajectory(Entity_t *thePlayer, Entity_t *playerBullet);

void updatePlayerBulletCoords(Entity_t *Bullet, Entity_t *thePlayer);    //WIP

void updateEnemyBulletCoords(Entity_t *Entity);     //WIP

void setHPLED(Entity_t *thePlayer);     //Sets HP LEDS based off player health

void collisionCheck(Entity_t *Player, Entity_t *Enemy); //Checks collisions

void crashCheck(Entity_t *Player, Entity_t *Enemy); //ends game if crash into enemy

void bulletCollisionCheck(Entity_t *Player, Entity_t *Bullet);  //WIP

uint8_t SwitchHandler(uint32_t A, uint32_t B, Entity_t *thePlayer); //Handles switch presses for player

void updateEnemyHP(Entity_t *Enemy);    //updates enemy hp on hit

void winHandler();

void gameEndHandler();

#endif
