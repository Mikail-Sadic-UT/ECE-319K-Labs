#ifndef GRAPHICS_H_
#define GRAPHICS_H_

void graphicsHandler(Entity_t *thePlayer, Entity_t *theEnemy, Entity_t *playerBullet);

void drawPlayer(Entity_t *thePlayer);   //Draws Player

void drawEnemy(Entity_t *theEnemy);     //Draws Enemy

void drawPlayerBullet(Entity_t *Bullet);    //Draws player bullet

void clearPlayerBullet();   //clears last instance of player bullet

void drawEnemyBullets();

void drawLangScrn();

void drawEngOpt();

void drawBHOpt();

void drawTitleEng();

void drawTitleBH();

void gameOverEng();

void gameOverBH();

void winEng();

void winBH();

void loreEng();

void loreBH();

void controlsEng();

void controlsBH();

void answersEng();

void answersBH();

#endif
