#ifndef ENGINE_H_
#define ENGINE_H_

struct Player
{
  uint8_t hp;
  uint8_t x;
  uint8_t y;
  const uint16_t *image; // img of sprite
  const uint8_t h, w;    // height and width of sprite
  int8_t spdX;
  int8_t spdY;
};
typedef struct Player Player_t;

struct Enemy
{
  uint16_t hp;
  uint8_t x;
  uint8_t y;
  const uint16_t *image; // img of sprite
  const uint8_t h, w;    // height and width of sprite
  int8_t spdX;
  int8_t spdY;
};
typedef struct Enemy Enemy_t;

struct PlayerBullet
{
  uint8_t x;
  uint8_t y;
  const uint16_t *image; // img of sprite
  const uint8_t h, w;    // height and width of sprite
  int8_t spdX;
  int8_t spdY;
};
typedef struct PlayerBullet PlayerBullet_t;

struct EnemyBullet
{
  uint8_t type;
  uint8_t x;
  uint8_t y;
  const uint16_t *image; // img of sprite
  const uint8_t h, w;    // height and width of sprite
  int8_t spdX;
  int8_t spdY;
};

typedef struct EnemyBullet EnemyBullet_t;

void playerInit(Player_t *thePlayer);

void setSpeed(Player_t *thePlayer);

void updateCoords(Player_t *thePlayer);

void setHPLED(Player_t *thePlayer);

uint8_t collisionCheck(Player_t *thePlayer);

uint8_t SwitchHandler(uint32_t A, uint32_t B, Player_t *thePlayer);

#endif
