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
#include "Graphics.h"

extern uint8_t score;
extern uint8_t FIRSTUPDATE;
extern uint8_t lastClear;
extern uint8_t ENEMYUPDATE;
extern uint8_t PLAYERUPDATE;
extern uint8_t ENEMYUPDATE;
extern uint8_t FIRSTUPDATE;
extern uint8_t bulletHit;
extern uint8_t WARP;
extern uint8_t pauseCount;

extern Entity_t enemyBullets[];

void graphicsHandler(Entity_t *thePlayer, Entity_t *theEnemy, Entity_t *playerBullet){
    drawPlayer(thePlayer);
    drawEnemy(theEnemy);
    if(playerBullet->live >= 1) drawPlayerBullet(playerBullet);
    if(lastClear) clearPlayerBullet();
    FIRSTUPDATE = 0;
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

void drawIndicator(){
    if(WARP) ST7735_DrawBitmap(1, 6, indicatorgreen, 5, 5);
    else ST7735_DrawBitmap(1, 6, indicator, 5, 5);
}

#define enemyBulletBuffer 64
void drawEnemyBullets(){
    for(uint8_t i = 0; i < enemyBulletBuffer; i++){
        if(enemyBullets[i].live >= 1){
            ST7735_DrawBitmap(enemyBullets[i].yOld, enemyBullets[i].xOld, enemy_BulletOld, 5, 5);
            if(enemyBullets[i].type == 1) ST7735_DrawBitmap(enemyBullets[i].y, enemyBullets[i].x, red_Bullet, 5, 5);
            else if(enemyBullets[i].type  == 2) ST7735_DrawBitmap(enemyBullets[i].y, enemyBullets[i].x, blue_Bullet, 5, 5);
            enemyBullets[i].xOld = enemyBullets[i].x;
            enemyBullets[i].yOld = enemyBullets[i].y;
        } else if(enemyBullets[i].lastClear == 1){
            ST7735_DrawBitmap(enemyBullets[i].yHit, enemyBullets[i].xHit, enemy_BulletOld, 5, 5);
            enemyBullets[i].lastClear = 0;
        }
    }
}

void drawLangScrn(){        //Language select screen
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Language", 2, ST7735_WHITE);
    ST7735_SetCursor(11, 5);
    ST7735_OutStringCool("English", 1, ST7735_WHITE);
    ST7735_SetCursor(7, 7);
    ST7735_OutStringCool("Srpsko-Hrvatski   ", 1, ST7735_WHITE);
  //ST7735_OutStringCool("Bosanski / Hrvatski", 1, ST7735_WHITE);
    ST7735_SetCursor(9, 10);
    ST7735_OutStringCool("Jezik", 2, ST7735_WHITE);

    ST7735_DrawBitmap(47, 111, iconup, 11, 11);
    ST7735_DrawBitmap(68, 137, icondwn, 11, 11);
}

void drawEngOpt(){
    ST7735_SetCursor(7, 1);
    ST7735_OutStringCool("Options", 2, ST7735_WHITE);
    ST7735_SetCursor(4, 3);
    ST7735_OutStringCool("       PlyrHP BossHP", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 4);
    ST7735_OutStringCool("Demo     13    min", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 5);
    ST7735_OutStringCool("Easy      7    low", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 6);
    ST7735_OutStringCool("Normal    5    med", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 7);
    ST7735_OutStringCool("Hard      3    high", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 9);
    ST7735_OutStringCool("No-Hit    1    most", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Press any button to", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("      return", 1, ST7735_WHITE);

    ST7735_DrawBitmap(35, 155, iconup, 11, 11);
    ST7735_DrawBitmap(47, 155, iconrt, 11, 11);
    ST7735_DrawBitmap(59, 155, icondwn, 11, 11);
    ST7735_DrawBitmap(71, 155, iconlft, 11, 11);
    ST7735_DrawBitmap(89, 155, joystick, 10, 10);
}

void drawBHOpt(){
    ST7735_SetCursor(7, 1);
        ST7735_OutStringCool("Opcije", 2, ST7735_WHITE);
        ST7735_SetCursor(4, 3);
        ST7735_OutStringCool("       IgrcHP SefHP", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 4);
        ST7735_OutStringCool("Demo     13    manje", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 5);
        ST7735_OutStringCool("Lako      7    malo", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 6);
        ST7735_OutStringCool("Normalno  5   srednje", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 7);
        ST7735_OutStringCool("Tesko     3    puno", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 9);
        ST7735_OutStringCool("Bez-Udar  1    vise", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 11);
        ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
        ST7735_SetCursor(4, 12);
        ST7735_OutStringCool("     se vratis     ", 1, ST7735_WHITE);

        ST7735_DrawBitmap(35, 155, iconup, 11, 11);
        ST7735_DrawBitmap(47, 155, iconrt, 11, 11);
        ST7735_DrawBitmap(59, 155, icondwn, 11, 11);
        ST7735_DrawBitmap(71, 155, iconlft, 11, 11);
        ST7735_DrawBitmap(89, 155, joystick, 10, 10);
}

void drawTitleEng(){
    ST7735_DrawBitmap(13, 143, TITLE, 15, 127);
    ST7735_SetCursor(11, 4);
    ST7735_OutStringCool("Start", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 6);
    ST7735_OutStringCool("Options", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 8);
    ST7735_OutStringCool("Controls", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 10);
    ST7735_OutStringCool("Lore", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 111, iconrt, 11, 11);
    ST7735_DrawBitmap(57, 111, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 111, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 111, iconup, 11, 11);

    ST7735_SetCursor(3, 12);
    ST7735_OutStringCool("Info", 1, ST7735_WHITE);
    ST7735_DrawBitmap(117, 156, joystick, 10, 10);

}

void drawTitleBH(){
    ST7735_DrawBitmap(13, 143, TITLE, 15, 127);
    ST7735_SetCursor(11, 4);
    ST7735_OutStringCool("Pocni", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 6);
    ST7735_OutStringCool("Opcije", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 8);
    ST7735_OutStringCool("Kontrole", 1, ST7735_WHITE);
    ST7735_SetCursor(11, 10);
    ST7735_OutStringCool("Prica", 1, ST7735_WHITE);
    ST7735_DrawBitmap(37, 111, iconrt, 11, 11);
    ST7735_DrawBitmap(57, 111, icondwn, 11, 11);
    ST7735_DrawBitmap(77, 111, iconlft, 11, 11);
    ST7735_DrawBitmap(97, 111, iconup, 11, 11);

    ST7735_SetCursor(3, 12);
    ST7735_OutStringCool("Info", 1, ST7735_WHITE);
    ST7735_DrawBitmap(117, 156, joystick, 10, 10);
}

void gameOverEng(){
    ST7735_SetCursor(4, 1);
    ST7735_OutStringCool("GAME OVER!", 2, ST7735_RED);
    ST7735_SetCursor(10, 7);
    ST7735_OutStringCool("Score:", 1, ST7735_WHITE);
    printf("%i", score + pauseCount);

}

void gameOverBH(){
    ST7735_SetCursor(2, 1);
    ST7735_OutStringCool("IGRA GOTOVA!", 2, ST7735_RED);
    ST7735_SetCursor(9, 7);
    ST7735_OutStringCool("Rezultat:", 1, ST7735_WHITE);
    printf("%i", score + pauseCount);
}

void winEng(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("YOU WIN!", 2, ST7735_GREEN);
    ST7735_SetCursor(9, 7);
    ST7735_OutStringCool("Score:", 1, ST7735_WHITE);
    printf("%i", score + pauseCount);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Press any button to", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     continue", 1, ST7735_WHITE);
}

void winBH(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("POBJEDA!", 2, ST7735_GREEN);
    ST7735_SetCursor(8, 7);
    ST7735_OutStringCool("Rezultat:", 1, ST7735_WHITE);
    printf("%i", score + pauseCount);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     nastavis     ", 1, ST7735_WHITE);
}

void infoEng(){
    ST7735_SetCursor(10, 1);
    ST7735_OutStringCool("INFO", 2, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("Score is determined by", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("damage done + num of", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("remaining pauses. Max ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("score is 100. Top left num", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("during game is Boss HP.", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 8);
    ST7735_OutStringCool("Top right is warp ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool("indicator. The LEDs will ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 10);
    ST7735_OutStringCool("show last 3 player lives.", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Press any button to", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("      return", 1, ST7735_WHITE);
}

void infoBH(){
    ST7735_SetCursor(10, 1);
    ST7735_OutStringCool("INFO", 2, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("Rezultat je determinan", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("od broj udaraca i sta je", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("ostala od pauza. Max ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("rezultat je 100. Broj gore", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("ljevo dok igras je Sef HP.", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 8);
    ST7735_OutStringCool("Gore desno je warp ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool("indikaotr. LEDove ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 10);
    ST7735_OutStringCool("prikazuju zadnja 3 zivota.", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     se vratis     ", 1, ST7735_WHITE);
}

void answersEng(){
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(1, 1);
    ST7735_OutStringCool("Answer Sheet", 2, ST7735_GREEN);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("Note to self:", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("Remember to make", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("answer key", 1, ST7735_WHITE);
    ST7735_SetCursor(10, 8);
    ST7735_OutStringCool("~Jon Valvano :)", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 11);
    ST7735_OutStringCool("Press any button to reset", 1, ST7735_WHITE);
}

void answersBH(){
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Odgovore", 2, ST7735_GREEN);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("Nemoj zaboravit:", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("Napravi odgovore", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("za ispit", 1, ST7735_WHITE);
    ST7735_SetCursor(10, 8);
    ST7735_OutStringCool("~Jon Valvano :)", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     resetujes     ", 1, ST7735_WHITE);
}

void controlsEng(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Controls", 2, ST7735_WHITE);
    ST7735_SetCursor(5, 3);
    ST7735_OutStringCool("Shoot - Auto aim", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 5);
    ST7735_OutStringCool("Speed shift - Fast", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 7);
    ST7735_OutStringCool("Warp - Teleport ~5sec", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 9);
    ST7735_OutStringCool("Pause - Pause game", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Press any button to", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     continue", 1, ST7735_WHITE);
    ST7735_DrawBitmap(27, 151, iconrt, 11, 11);
    ST7735_DrawBitmap(47, 151, icondwn, 11, 11);
    ST7735_DrawBitmap(67, 151, iconlft, 11, 11);
    ST7735_DrawBitmap(87, 151, iconup, 11, 11);
}

void controlsBH(){
    ST7735_SetCursor(6, 1);
    ST7735_OutStringCool("Kontrole", 2, ST7735_WHITE);
    ST7735_SetCursor(5, 3);
    ST7735_OutStringCool("Pucaj - Auto aim", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 5);
    ST7735_OutStringCool("Mjen brzine - Birze", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 7);
    ST7735_OutStringCool("Warp - Teleport ~5sec", 1, ST7735_WHITE);
    ST7735_SetCursor(5, 9);
    ST7735_OutStringCool("Pauza - Zaustavi igru", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 11);
    ST7735_OutStringCool("Pritizni sta god da", 1, ST7735_WHITE);
    ST7735_SetCursor(4, 12);
    ST7735_OutStringCool("     nastavis     ", 1, ST7735_WHITE);
    ST7735_DrawBitmap(27, 151, iconrt, 11, 11);
    ST7735_DrawBitmap(47, 151, icondwn, 11, 11);
    ST7735_DrawBitmap(67, 151, iconlft, 11, 11);
    ST7735_DrawBitmap(87, 151, iconup, 11, 11);
}

void loreEng(){
    ST7735_SetCursor(1, 0);
    ST7735_OutStringCool("Word has it that Valvano ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 1);
    ST7735_OutStringCool("will make the final impos-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 2);
    ST7735_OutStringCool("-sible. He has fled in his", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("spaceship to avoid being", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("hunted down. You have ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("decided to hunt him. Beat", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("him to get the answer key", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("and save the 319k students", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool("  You are the last hope.  ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 11);
    ST7735_OutStringCool("    Press any button", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 12);
    ST7735_OutStringCool("       to return", 1, ST7735_WHITE);
}

void loreBH(){
    ST7735_SetCursor(1, 0);
    ST7735_OutStringCool("Izgleda da ce Valvano", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 1);
    ST7735_OutStringCool("napravit zadnji ispit nem-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 2);
    ST7735_OutStringCool("-oguce. Pobjego je u svem-", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 3);
    ST7735_OutStringCool("-irski brod da ga ne bi", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 4);
    ST7735_OutStringCool("lovili studente. Ti ces", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 5);
    ST7735_OutStringCool("ga uhvatit. Pobjedi ga da", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 6);
    ST7735_OutStringCool("dobjes odgovore za ispit,", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 7);
    ST7735_OutStringCool("i da spasis 319k studente", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 9);
    ST7735_OutStringCool(" Ti si zadnji sto mozes. ", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 11);
    ST7735_OutStringCool("    Pritizni sta god", 1, ST7735_WHITE);
    ST7735_SetCursor(1, 12);
    ST7735_OutStringCool("      da se vratis", 1, ST7735_WHITE);
}
