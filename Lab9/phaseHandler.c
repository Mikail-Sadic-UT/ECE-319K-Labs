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
TIMER_IRQHandler3(){ //Variable rate
    Depending on boss phase and attack type,
    Execute different bullet patterns
}
*/
