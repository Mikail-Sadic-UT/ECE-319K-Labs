// Sound.h
// Sound assets based off the original Space Invaders
// Jonathan Valvano
// 11/15/2021 
#ifndef __SOUND_H
#define __SOUND_H
#include <stdint.h>

const uint8_t Crash[3082] = {15,17,16,15,15,16,15,15,15,15,16,16,16,17,17,14,14,17,15,14,15,15,15,16,15,13,14,17,18,14,15,17,17,17,14,13,17,17,15,17,15,15,16,16,16,15,16,15,15,18,17,14,13,16,18,17,15,14,19,19,15,14,14,18,16,14,19,13,18,21,7,11,16,8,8,18,26,23,22,26,27,25,6,9,29,13,10,27,15,2,3,2,2,0,3,20,22,4,13,26,4,0,7,2,8,11,14,31,30,22,31,21,0,3,9,18,32,30,29,31,27,24,17,14,16,16,18,12,10,25,18,-1,1,1,0,9,8,15,30,17,2,6,4,6,12,9,16,29,32,26,8,-1,14,29,29,27,25,26,23,19,25,21,15,17,12,12,12,2,-2,2,8,8,2,4,9,7,7,11,13,13,10,10,19,22,19,20,13,12,16,12,18,22,24,31,30,31,30,29,30,15,6,12,11,16,25,27,28,30,30,18,4,-1,2,16,30,29,29,31,27,23,21,6,2,16,15,13,16,8,15,21,6,2,-1,2,17,12,11,19,13,13,17,12,11,7,4,3,1,2,1,7,13,7,9,11,15,27,18,9,12,7,15,27,28,31,29,28,24,24,30,15,1,6,5,7,16,15,15,15,14,15,12,13,16,8,11,28,32,30,31,30,25,21,15,5,5,12,12,17,28,33,26,9,1,6,17,18,17,27,27,22,22,16,22,22,3,-2,2,-2,5,18,19,20,24,22,17,8,6,17,17,11,7,5,16,21,19,21,17,7,-1,1,4,1,4,9,12,24,30,26,26,21,12,9,4,2,5,7,21,23,21,30,16,5,16,18,22,20,13,23,25,23,19,2,4,18,21,28,28,9,10,21,10,7,14,12,24,31,17,21,26,11,13,21,12,16,33,28,18,22,15,0,7,12,2,13,29,16,11,31,26,11,19,8,-2,16,20,23,29,20,17,17,19,19,11,9,8,10,17,25,31,19,11,17,25,30,23,22,14,1,12,10,2,15,19,8,7,21,12,0,14,13,-1,3,15,20,22,29,16,1,4,0,5,13,11,8,3,3,5,10,12,7,21,20,10,27,32,30,32,19,11,18,23,11,17,31,15,14,20,19,20,10,16,14,4,11,3,12,26,19,16,4,4,16,20,22,14,11,7,6,24,25,23,25,21,28,29,26,20,17,18,8,12,24,26,30,26,22,23,24,29,28,24,24,21,15,18,19,7,7,10,2,0,-1,5,12,14,19,10,0,5,14,17,4,2,6,2,18,23,11,15,19,11,9,19,21,24,31,28,21,10,14,24,13,15,13,3,11,20,31,24,16,27,13,13,28,16,18,24,17,9,0,9,13,14,18,4,0,2,0,4,1,1,3,1,10,21,31,30,18,13,3,2,20,21,20,22,10,10,24,29,22,12,9,10,12,21,30,29,19,18,22,21,24,28,30,28,26,25,16,17,25,14,12,18,12,18,26,24,20,15,14,13,16,22,18,19,20,6,-2,2,4,4,2,-2,4,8,10,18,9,1,3,0,2,1,11,23,14,18,19,10,24,27,17,9,8,15,16,26,33,18,14,14,17,29,32,30,29,27,23,11,10,20,23,17,13,11,13,21,28,29,20,2,7,24,27,30,32,16,5,3,0,11,26,24,18,15,9,6,8,12,3,-3,3,-2,4,22,23,12,2,7,11,4,11,26,27,24,23,18,22,29,21,14,16,15,13,21,27,16,2,0,6,24,29,21,28,25,14,9,2,11,22,16,11,12,13,18,23,17,5,2,2,6,20,30,29,30,31,13,4,17,13,10,15,11,16,19,17,20,14,3,0,7,17,22,22,17,19,18,12,21,19,18,29,24,17,16,14,18,20,11,2,4,10,21,25,9,0,1,1,2,4,17,26,26,30,21,14,18,19,20,17,14,14,18,30,30,20,11,7,11,14,14,19,30,26,10,13,17,14,25,26,20,24,28,30,30,31,30,27,23,22,21,17,16,15,15,9,6,11,7,1,5,2,-1,2,3,1,1,7,2,3,19,24,25,20,8,5,2,1,9,16,12,6,10,10,10,16,16,15,12,8,13,24,32,25,22,25,16,17,24,26,28,22,26,29,23,27,27,22,21,16,12,19,27,29,30,24,13,11,10,14,24,18,12,20,19,13,15,9,7,18,13,11,23,25,23,12,2,5,7,12,15,14,10,0,1,4,0,0,0,0,2,4,10,18,23,17,13,12,11,21,29,31,26,18,16,14,18,24,22,22,18,21,24,10,19,29,14,16,15,8,25,33,27,19,8,2,4,17,24,16,14,17,19,20,10,7,12,5,2,10,17,29,33,27,21,16,17,16,16,21,19,16,14,14,17,13,11,7,1,2,0,11,20,13,13,9,4,6,9,20,19,15,15,10,18,20,17,25,20,15,18,22,30,28,27,21,15,20,16,20,31,28,20,13,6,11,21,18,14,23,20,20,31,27,22,15,4,1,0,5,10,14,16,7,3,14,22,19,9,8,14,13,14,25,23,17,15,4,9,16,6,13,18,11,11,3,6,22,18,9,11,10,9,15,18,17,17,18,20,27,33,29,26,24,12,1,-1,6,16,18,19,17,16,20,20,24,23,14,13,16,23,31,31,30,27,19,8,7,15,21,22,12,9,15,10,13,18,15,11,4,15,31,21,10,9,7,1,-1,8,15,23,26,18,12,4,4,17,15,9,11,16,21,22,24,19,12,15,11,8,18,28,31,27,20,12,9,16,22,21,20,21,20,21,20,11,4,-1,1,7,7,14,24,21,18,8,-1,4,7,15,18,11,18,23,21,26,24,17,17,23,27,30,29,27,27,21,10,1,11,22,16,22,24,8,13,17,13,14,5,9,16,14,23,16,7,8,2,1,-1,9,25,22,18,16,7,4,7,8,8,8,10,20,27,22,21,18,17,19,12,15,26,30,32,27,18,10,10,20,26,25,20,20,24,25,21,8,6,14,13,17,23,25,26,21,16,6,0,10,16,17,17,10,0,5,14,5,0,5,5,13,20,15,19,18,4,3,7,0,7,22,20,22,24,14,17,20,11,5,9,8,8,19,18,13,21,18,11,18,20,26,32,29,28,18,14,24,21,21,22,15,29,29,17,25,15,-1,7,7,6,20,29,28,24,15,3,4,15,20,16,14,14,15,25,24,12,14,14,12,20,24,28,27,25,22,11,6,10,14,19,15,7,0,5,15,8,1,-1,4,11,15,25,29,20,16,12,5,8,19,27,30,24,10,9,12,15,19,15,17,20,21,29,30,27,18,8,13,11,13,30,32,28,22,11,10,10,9,11,5,4,15,21,24,25,14,8,11,11,13,17,26,32,22,16,12,7,12,16,13,8,8,11,12,13,6,0,1,0,8,17,21,26,23,15,5,-1,2,4,13,16,12,14,17,23,26,23,17,7,12,26,29,31,30,25,20,16,14,20,28,29,25,17,16,19,14,14,11,1,3,13,21,28,31,18,2,9,8,1,15,20,18,14,4,4,6,8,4,-1,2,0,12,25,21,22,9,0,7,10,17,27,32,29,23,20,15,20,28,24,18,19,27,29,30,30,13,1,-1,0,10,18,25,17,11,12,2,2,10,13,18,17,17,21,28,27,15,9,2,1,17,25,29,30,20,11,8,9,5,12,20,12,14,18,13,15,18,14,5,6,10,17,31,31,21,19,11,8,15,19,20,27,26,13,12,11,4,7,6,1,0,9,22,20,22,17,1,10,14,8,22,26,18,17,14,14,19,23,24,22,25,23,22,29,25,18,14,5,9,26,28,21,24,27,15,11,17,18,20,20,18,20,19,22,25,19,15,5,1,13,21,25,22,15,15,8,-1,3,9,9,4,9,16,15,21,18,8,6,6,13,23,29,26,19,19,13,17,22,12,17,18,7,11,9,8,12,13,8,1,12,18,16,26,22,17,16,9,13,21,25,21,18,22,15,14,19,20,14,8,9,10,18,26,22,13,2,1,2,1,11,8,9,18,9,12,21,21,18,11,16,21,19,28,31,25,18,10,8,17,29,29,25,21,18,14,10,14,8,3,12,11,14,20,21,23,13,3,3,12,30,31,30,32,22,17,13,13,19,15,12,16,20,18,19,21,10,4,3,1,11,24,28,22,16,15,9,17,20,14,21,20,18,24,26,29,28,24,15,8,16,19,22,24,12,4,1,1,3,11,12,1,6,8,2,8,6,1,2,2,6,13,31,30,21,21,9,14,25,20,25,28,30,25,22,27,15,21,22,4,12,12,12,24,25,16,3,3,4,8,27,21,17,27,26,25,25,23,13,11,15,11,18,20,15,17,5,-1,1,3,12,14,13,8,4,9,9,11,8,5,12,17,22,19,18,19,11,11,10,10,18,24,29,25,18,11,13,20,11,16,22,17,29,29,24,26,17,17,15,18,23,18,28,30,20,13,7,15,17,14,18,15,18,22,22,16,11,15,8,4,13,15,17,17,11,2,-1,1,4,15,9,2,6,3,8,6,1,7,4,9,17,21,31,31,30,22,10,8,13,29,31,23,20,11,19,24,14,11,6,9,13,22,31,26,27,22,15,16,10,19,29,29,26,8,5,15,15,12,4,6,12,14,22,20,15,7,4,12,6,4,15,26,32,21,13,10,14,21,12,10,20,17,16,27,28,17,14,14,11,17,17,20,28,19,11,3,1,11,7,3,11,12,13,19,17,8,6,6,4,10,18,20,17,21,23,10,5,11,15,19,15,16,22,24,31,29,20,15,18,22,24,31,29,18,17,13,4,-1,9,23,20,21,24,22,25,24,18,9,5,11,16,29,30,17,14,9,7,4,1,6,6,10,13,9,10,9,9,6,1,3,4,12,20,20,13,8,12,12,14,21,24,28,31,30,25,23,28,24,23,24,18,21,28,30,21,9,7,7,12,18,15,12,14,16,11,5,2,6,10,4,5,13,12,16,17,10,6,3,10,20,25,30,24,24,30,20,15,20,17,18,24,26,24,23,23,20,14,7,1,9,21,20,17,9,4,13,11,3,7,10,9,16,21,12,8,13,8,0,4,10,13,19,24,16,8,13,14,16,25,24,23,31,30,28,30,20,18,22,15,15,24,24,24,25,12,4,10,9,8,11,9,6,9,18,13,0,8,10,3,10,12,13,24,26,19,15,15,18,25,26,19,18,15,12,14,8,2,3,12,14,11,20,22,20,27,16,2,9,15,15,18,16,9,9,14,15,13,16,14,19,31,30,24,24,20,22,21,10,8,16,28,26,19,17,10,19,27,17,21,24,21,24,25,17,5,9,11,3,9,11,11,18,13,5,1,0,0,7,11,5,8,14,15,19,7,0,7,11,21,23,24,30,26,28,21,4,12,26,25,20,22,24,26,27,18,11,12,15,20,23,28,28,25,26,17,9,9,6,12,19,13,11,11,8,13,14,4,3,11,16,25,28,17,11,19,19,7,14,20,14,25,26,14,14,19,20,15,12,12,13,23,18,6,5,0,0,6,8,6,8,19,14,9,15,7,8,21,18,15,17,20,26,29,18,5,10,15,15,20,22,20,21,23,20,13,19,23,22,24,22,21,21,20,19,10,6,5,10,21,22,22,24,19,18,22,11,4,17,20,12,15,16,13,18,19,10,8,16,23,31,29,22,23,21,18,13,7,10,10,15,22,14,14,14,12,15,9,7,10,15,22,17,11,3,5,15,10,11,17,14,18,22,15,5,7,10,1,5,12,17,28,29,24,19,14,13,14,20,20,22,28,21,21,21,11,8,11,12,11,17,25,25,27,18,8,9,11,15,14,19,20,11,12,12,9,9,6,11,14,16,20,19,20,17,15,9,9,18,18,20,20,10,9,5,7,9,5,12,17,22,30,30,27,23,23,19,13,16,21,26,21,13,11,4,9,13,9,13,11,16,22,23,24,13,16,25,13,17,29,29,32,28,18,11,10,11,10,9,6,9,15,14,12,14,12,6,10,13,11,21,23,15,16,8,4,10,15,14,10,15,14,9,17,14,9,12,11,15,21,25,31,29,21,15,14,12,11,18,20,18,15,13,12,12,12,4,1,4,6,16,23,22,19,13,12,10,12,21,22,22,22,19,19,20,27,25,19,20,19,27,31,29,26,11,6,7,6,14,17,19,18,14,15,9,11,14,8,10,7,11,21,20,15,7,5,2,4,16,19,19,16,10,12,4,1,7,9,15,15,12,17,24,24,16,10,9,12,20,26,31,30,26,21,15,21,23,23,23,16,14,14,16,17,9,6,6,11,18,19,28,31,19,7,6,16,18,15,16,17,20,19,15,13,17,18,7,10,15,15,28,29,17,8,4,9,8,10,13,9,14,15,10,8,7,8,3,1,3,8,25,25,18,20,6,7,21,18,19,26,27,30,31,24,19,23,23,17,18,21,25,29,25,12,5,4,0,7,18,20,17,13,11,10,9,7,4,12,17,18,27,30,29,25,14,7,6,9,17,21,18,13,9,7,10,11,9,15,14,16,21,18,22,21,10,8,9,12,18,26,29,20,13,8,8,9,8,16,16,16,23,19,21,22,12,6,5,12,22,24,27,26,17,8,7,10,14,18,20,20,16,18,20,13,12,12,7,13,23,29,30,31,27,12,7,8,8,18,18,11,14,17,17,16,15,9,4,5,6,12,20,19,17,15,8,3,14,24,23,24,19,13,16,14,9,10,13,11,12,24,26,22,21,11,5,4,0,11,26,27,22,21,18,16,17,16,15,16,18,20,24,31,20,6,7,1,-1,6,12,23,23,15,9,5,11,12,12,16,18,25,29,30,31,23,15,16,16,12,18,24,20,17,11,6,8,13,20,20,23,25,19,19,19,14,10,5,10,13,16,19,15,13,6,0,1,0,9,15,13,17,15,15,16,15,18,13,12,16,18,26,28,23,19,17,16,16,25,27,25,25,17,16,15,14,17,13,12,13,15,21,19,17,13,5,8,8,11,22,24,20,16,13,14,13,15,19,18,14};

const uint8_t Hit[375] = {15,16,16,15,16,15,16,16,15,16,15,17,16,15,16,15,16,16,15,15,14,16,16,17,17,14,15,15,14,17,15,18,16,14,17,16,16,17,15,15,13,15,15,14,16,13,12,16,19,18,18,22,20,23,25,18,18,18,18,18,12,17,17,11,16,18,13,7,9,14,11,17,18,7,8,15,16,18,26,30,17,15,18,3,12,21,9,21,12,2,31,19,10,24,2,5,31,32,29,16,11,16,26,29,6,-1,2,-1,4,0,9,19,19,25,17,13,17,16,27,31,26,24,26,22,9,17,20,3,2,3,0,3,5,10,11,15,21,18,19,18,17,18,21,28,22,18,24,21,21,22,21,22,24,26,21,17,16,10,14,20,17,18,22,23,21,23,22,16,12,9,5,4,0,0,3,1,5,10,10,11,16,18,15,18,20,19,19,16,16,18,14,14,17,15,16,17,14,11,14,15,15,16,16,16,17,17,17,18,18,19,20,17,16,17,18,18,17,17,15,12,12,13,13,13,15,15,16,16,16,17,18,16,16,16,15,15,17,16,14,13,13,12,12,13,14,12,10,12,14,13,15,16,13,16,18,17,17,18,19,17,16,18,17,16,17,16,15,15,15,14,14,14,13,14,15,16,16,17,18,16,15,16,16,15,16,16,16,15,15,16,15,16,15,15,15,16,15,16,16,16,16,16,16,16,15,16,16,15,16,16,16,15,15,15,15,15,15,15,16,16,16,15,15,15,15,16,16,16,16,15,15,15,16,16,15,16,16,16,16,16,16,16,16,15,16,16,15,16,16,16,16,16,16,16,16,15,15,16,15,16,16,16,15,15,16,15,15,16,16,16,16,15,15,15,15};

const uint8_t Warp[4205] = {15,15,16,16,16,16,16,17,17,18,18,18,19,19,18,18,18,19,18,17,17,17,16,16,16,16,16,15,14,14,14,13,14,14,14,13,13,14,14,13,14,14,15,15,14,13,13,14,15,14,14,13,11,11,12,11,12,11,11,10,9,9,9,8,8,8,6,5,4,6,5,3,3,3,3,3,3,4,5,5,5,6,8,9,8,9,11,12,12,13,15,17,17,17,18,19,20,20,19,20,21,22,22,21,21,21,21,21,22,23,23,22,20,19,19,19,19,19,19,18,18,18,18,18,18,17,17,15,15,15,15,16,17,16,15,13,12,13,14,14,13,13,14,14,12,12,12,12,12,12,12,12,13,14,16,17,17,18,19,20,20,20,19,21,22,22,21,22,23,24,23,23,22,20,18,20,20,21,22,22,22,21,20,21,22,22,21,20,20,20,19,18,18,19,17,16,15,15,17,17,18,17,14,14,14,14,15,16,18,17,16,16,17,18,17,16,17,18,19,19,20,22,21,21,22,22,22,23,27,29,30,29,28,29,30,29,30,30,31,31,30,30,30,30,30,30,31,31,30,28,28,29,30,29,29,30,29,25,21,20,21,22,22,20,19,20,18,14,11,11,11,12,12,10,9,9,10,11,8,8,7,4,4,5,6,8,9,10,10,10,11,9,7,6,4,5,4,3,4,5,9,10,10,11,11,11,12,13,13,15,14,14,17,17,17,18,17,19,20,18,18,14,14,15,15,19,20,19,19,16,16,16,16,17,14,14,16,14,15,15,11,10,7,4,4,6,8,12,14,14,13,11,12,13,13,17,17,17,18,16,18,20,18,20,19,15,15,19,20,23,25,24,22,23,22,23,26,26,24,26,27,27,29,29,29,31,28,30,31,28,29,28,26,26,27,29,27,27,27,23,24,26,24,28,31,31,31,31,31,30,31,31,29,29,27,23,25,26,22,23,26,27,30,31,31,31,29,28,23,18,18,14,12,14,14,16,16,15,19,17,17,20,19,19,20,17,16,19,18,13,13,15,12,10,11,12,15,15,15,18,16,15,17,15,13,15,15,15,13,13,15,14,13,17,18,17,18,20,19,19,21,21,20,20,19,19,17,14,15,16,14,13,16,15,12,16,19,16,18,21,21,22,24,24,26,27,26,26,27,26,23,22,19,14,12,13,12,11,13,16,17,17,17,18,20,20,19,19,19,17,13,12,12,9,8,9,8,9,9,6,6,9,8,7,9,9,9,9,11,12,9,11,17,19,21,23,24,25,25,25,24,24,25,24,21,20,20,18,15,14,15,12,10,10,11,8,4,2,2,2,2,1,2,5,9,13,15,16,18,17,17,19,21,21,20,18,17,16,15,14,14,17,20,17,13,13,11,7,2,0,1,2,0,0,0,2,3,6,9,11,15,19,20,20,24,26,26,27,26,24,23,19,18,20,21,19,17,17,16,15,16,19,20,22,25,26,23,22,23,23,22,21,22,26,27,23,22,23,24,24,21,21,20,19,18,17,14,15,17,16,15,13,11,11,9,6,7,11,14,13,13,16,16,16,18,22,22,20,22,26,26,23,22,23,23,21,19,18,19,20,16,13,14,15,14,13,11,9,9,9,10,10,10,12,12,9,9,9,8,8,8,6,4,5,8,10,9,11,15,15,14,15,17,16,15,16,18,18,17,17,19,18,15,13,12,14,17,17,15,14,16,19,20,20,19,19,18,19,20,19,17,17,17,17,16,16,16,14,13,11,8,8,9,9,9,7,8,11,9,4,2,3,4,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,6,10,11,11,12,14,17,19,20,21,23,27,28,29,30,31,31,31,31,31,30,31,31,31,31,31,31,31,31,31,31,31,31,31,30,30,31,31,31,30,30,31,30,27,24,23,23,25,25,22,19,18,19,20,20,17,14,13,14,15,14,13,12,14,16,16,15,15,17,17,17,16,15,15,15,18,20,19,17,16,18,22,22,19,16,16,18,20,19,19,20,21,21,21,22,22,22,21,19,20,21,23,25,24,22,22,23,25,27,26,23,22,23,25,27,25,23,23,25,25,24,23,22,23,23,21,19,17,15,16,16,15,13,10,9,10,11,10,7,3,2,3,6,5,2,1,3,5,5,3,2,2,2,2,0,0,0,1,4,5,3,1,1,4,7,7,4,3,5,7,9,9,8,8,10,12,15,14,13,13,16,19,20,19,18,18,19,22,25,25,24,25,26,29,31,31,31,29,28,29,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,31,31,31,28,25,23,23,23,24,22,19,16,16,17,17,16,14,13,13,14,14,14,12,11,11,12,12,11,10,7,7,9,10,10,8,7,6,8,10,11,11,8,5,5,8,11,11,9,7,8,10,12,12,11,10,10,10,10,11,12,13,14,17,16,15,16,17,20,24,24,22,21,22,25,28,28,26,24,24,27,29,29,28,26,26,28,29,29,28,26,25,26,27,28,27,25,24,24,25,26,25,24,24,22,21,21,21,22,22,22,22,21,21,21,20,20,18,16,15,13,12,12,13,13,12,10,8,7,7,8,8,6,4,2,1,2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,4,3,1,0,1,3,5,6,7,7,7,8,9,10,10,9,8,6,7,8,9,10,10,9,9,9,9,10,11,11,11,10,10,10,10,13,13,12,11,10,11,14,15,16,14,13,14,16,18,18,18,17,16,17,19,21,22,21,20,21,23,25,26,27,26,25,26,27,28,27,27,27,29,31,31,31,30,30,30,31,31,30,29,28,28,30,31,31,31,30,29,30,30,31,31,30,29,28,28,29,29,28,27,25,25,26,27,27,26,24,22,22,22,22,21,20,19,17,16,16,15,15,15,13,13,13,13,13,12,12,11,11,10,10,9,8,7,8,9,11,12,11,10,9,10,11,12,11,9,8,8,9,11,12,11,10,10,10,11,10,10,11,11,11,10,10,10,11,12,13,12,12,12,13,14,15,15,13,12,12,14,15,16,16,16,14,14,16,18,19,20,20,19,19,21,23,24,24,24,23,24,26,27,27,26,25,26,29,31,31,30,28,28,29,31,31,30,29,29,30,31,31,31,31,31,30,31,31,31,31,31,30,29,30,30,30,31,30,29,28,28,29,31,30,29,28,27,28,28,29,29,28,27,26,26,25,26,27,27,26,24,24,24,25,25,25,24,23,23,24,24,23,21,19,19,20,21,22,21,19,16,15,16,16,16,14,11,9,10,10,11,10,9,7,7,7,8,8,7,6,6,6,7,7,7,7,7,7,7,9,10,10,10,9,9,9,10,10,10,11,11,11,12,11,11,10,10,10,11,12,12,11,11,11,12,12,12,12,11,11,12,13,13,12,10,10,12,14,14,14,13,12,13,14,15,14,12,11,12,13,14,15,14,13,13,14,14,14,13,14,14,14,15,15,14,13,14,14,14,14,13,13,14,15,15,14,14,14,14,15,15,15,15,15,14,13,12,12,14,14,14,14,14,14,15,16,16,15,15,15,16,17,18,17,17,15,15,17,18,19,18,17,16,16,17,18,17,16,14,15,16,16,16,16,15,15,15,15,16,16,16,16,16,16,17,17,17,18,18,19,19,19,20,21,22,22,22,22,22,23,24,24,24,25,26,27,27,26,26,26,28,30,30,30,29,29,29,29,30,30,30,29,28,29,30,29,29,28,27,28,30,31,30,29,29,29,29,30,30,30,29,29,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,30,30,29,29,29,29,29,28,28,26,26,27,27,26,25,24,24,24,23,21,20,20,21,21,21,20,18,17,18,17,16,16,15,15,14,14,14,13,13,12,11,11,10,11,11,11,9,8,7,7,8,7,5,5,4,4,4,5,5,4,3,3,4,4,4,5,5,5,5,4,4,4,4,5,6,5,5,5,7,7,7,7,7,7,8,8,8,8,8,9,9,9,8,7,7,7,8,8,8,8,7,6,6,7,7,8,8,7,7,7,8,8,8,9,8,8,8,8,9,9,8,8,8,8,8,8,7,7,7,7,8,9,9,8,8,9,10,10,11,12,12,12,12,13,13,13,14,14,14,14,14,14,15,16,16,16,15,15,15,15,16,17,17,17,17,18,18,16,16,16,17,17,17,17,15,14,14,14,14,14,14,13,13,12,12,12,12,12,12,12,12,11,10,10,10,11,10,10,9,9,8,7,7,8,8,8,8,8,8,8,8,8,8,8,9,9,10,11,10,10,10,11,12,12,12,13,14,15,16,16,16,16,16,16,17,19,18,18,18,18,18,18,17,16,17,17,18,18,17,17,16,16,16,17,18,18,18,18,17,16,16,16,16,17,16,15,14,13,13,14,14,14,14,14,13,13,13,12,12,12,12,13,13,12,12,12,12,11,11,11,11,12,11,10,10,9,10,10,11,11,11,11,10,10,10,11,11,11,11,10,10,10,9,10,10,9,8,7,6,6,5,5,5,5,4,3,2,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,3,4,5,5,5,5,5,6,6,7,8,9,9,9,9,10,10,10,12,12,13,14,16,16,17,18,19,20,22,24,25,26,26,25,26,27,28,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,30,29,29,28,28,29,29,30,29,29,28,28,28,28,28,29,29,28,27,26,25,25,25,25,25,24,23,23,23,22,22,21,21,20,21,22,22,20,19,18,18,18,19,20,19,18,18,17,17,18,18,19,19,19,18,18,18,18,19,19,19,19,18,18,19,20,20,20,19,19,18,18,18,18,19,19,18,17,17,17,16,17,17,17,17,16,16,16,16,17,17,17,17,18,19,19,19,18,18,18,18,19,20,20,20,20,18,18,18,18,19,19,19,19,18,17,16,16,16,17,17,17,16,16,15,15,15,14,15,14,14,14,14,13,13,13,14,14,14,14,13,12,13,14,14,14,14,13,12,13,13,14,13,13,12,12,11,11,11,10,10,10,10,10,11,11,10,10,9,10,11,12,12,12,11,11,11,12,13,13,13,13,13,13,13,14,15,15,15,15,16,17,18,19,18,18,18,18,20,21,23,23,22,22,23,24,25,27,27,27,27,27,28,29,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,28,28,28,28,28,27,26,25,25,24,24,24,24,23,22,22,21,20,20,19,18,17,16,17,18,18,18,16,15,14,15,17,18,17,16,15,14,15,16,17,18,17,16,15,15,15,15,16,16,15,15,15,14,14,15,16,16,15,15,15,15,15,16,16,17,16,16,16,16,16,15,15,16,15,15,15,15,14,14,15,15,16,16,16,16,16,16,17,16,17,17,18,18,18,18,17,17,17,17,16,16,15,15,15,15,13,12,12,12,12,13,13,12,12,11,11,11,11,12,13,13,13,13,12,12,12,13,13,13,13,12,11,11,11,12,11,12,11,10,10,10,10,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,9,9,9,9,10,10,10,10,11,11,11,12,12,13,13,12,13,14,15,16,16,15,15,16,17,18,19,19,19,18,18,19,19,20,21,21,21,20,20,21,21,22,23,23,23,22,21,21,22,23,23,22,22,20,20,19,20,20,20,19,18,16,16,16,17,17,16,15,14,14,14,14,14,13,13,12,12,12,12,12,11,12,12,13,13,13,12,12,12,13,13,13,13,12,12,12,13,13,13,13,13,13,13,14,14,15,16,16,15,15,15,16,17,18,19,18,17,17,18,19,20,20,21,22,22,22,22,23,23,24,24,25,25,25,25,25,26,27,27,27,26,26,26,26,27,27,26,25,25,25,24,24,24,24,25,25,24,24,24,24,25,25,25,25,26,26,27,27,27,27,26,26,27,28,28,28,27,27,28,29,29,28,28,28,28,28,29,30,29,29,28,28,28,29,30,30,29,28,28,28,29,29,30,30,29,29,28,28,29,30,31,31,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,29,29,29,28,28,27,26,25,25,25,26,26,25,25,24,23,23,23,23,23,23,23,22,21,21,21,21,20,20,20,18,17,17,18,18,18,17,16,14,13,13,13,13,12,12,10,9,9,9,9,8,8,7,6,5,5,4,4,4,4,3,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,1,2,2,3,4,4,4,4,5,6,7,7,7,6,7,8,9,10,11,11,10,10,11,12,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,15,16,16,15,15,14,15,15,14,14,14,14,13,13,13,13,12,11,11,12,12,12,11,10,10,10,11,11,11,11,11,11,11,11,10,10,11,12,12,12,12,11,11,12,13,13,13,13,12,12,13,14,14,14,13,13,13,13,14,15,15,14,14,14,14,15,15,16,16,16,16,16,16,17,17,18,18,19,19,19,19,20,21,22,22,22,22,23,23,24,24,25,25,26,25,25,25,25,26,27,27,27,27,27,27,27,27,28,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,30,30,29,28,28,28,29,29,28,27,26,25,25,26,26,25,24,23,22,22,22,22,22,21,20,20,19,19,19,19,18,18,18,17,17,17,17,17,17,17,16,16,16,16,16,16,16,16,15,15,15,15,14,14,14,14,13,13,13,12,12,12,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,13,13,14,14,13,13,13,13,14,14,14,14,14,14,14,14,15,16,16,16,16,16,16,16,17,17,17,16,16,16,16,16,17,16,16,15,15,16,16,16,15,15,14,14,14,14,13,12,11,11,11,11,11,9,8,8,8,8,7,7,6,5,5,5,4,4,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,3,4,5,5,5,5,6,8,8,8,8,8,8,9,10,11,11,12,12,12,12,13,13,13,14,13,13,13,13,13,14,14,13,13,13,13,14,13,13,13,13,14,14,14,14,14,14,14,14,15,15,15,15,15,14,14,14,15,15,15,14,14,14,14,13,13,13,12,12,11,10,10,9,9,9,8,8,7,6,6,7,6,6,6,6,5,5,5,5,5,5,5,5,4,4,4,4,4,4,3,3,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,3,4,4,5,5,6,6,7,8,9,9,10,10,11,12,12,13,13,14,14,14,14,15,16,16,16,17,18,18,17,17,17,17,18,18,18,18,18,18,18,18,19,18,18,18,18,19,19,19,19,18,18,18,19,19,19,19,18,18,17,17,16,16,15,14,14,13,13,12,12,11,10,10,9,9,8,8,8,8,7,7,7,7,7,7,7,7,7,7,6,6,6,6,7,6,6,6,6,6,7,7,7,7,7,8,9,10,10,11,12,12,13,15,16,16,17,18,19,20,20,21,23,24,25,25,25,25,26,28,29,29,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,30,30,29,29,28,27,26,26,26,25,24,23,23,22,22,21,21,20,20,19,18,18,17,16,16,16,15,14,13,13,12,12,11,11,10,9,9,9,9,8,8,7,6,6,6,5,5,5,5,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,7,7,8,8,9,9,9,10,10,10,11,11,12,13,13,13,13,13,14,15,15,15,14,15,15,15,15,15,15,15,15,15,15,15,15,15,14,14,14,15,15,14,14,14,14,14,13,13,13,12,12,12,12,11,11,11,12,11,10,10,9,9,9,9,9,9,8,8,7,8,8,8,7,7,7,6,6,6,6,6,6,6,6,6,6,7,6,6,6,7,7,7,7,7,7,7,8,9,9,9,9,9,10,11,11,11,11,12,12,13,13,14,14,14,15,15,16,16,16,17,17,18,18,18,19,19,20,20,21,21,22,23,23,23,24,25,26,26,26,26,26,27,28,28,28,29,29,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,29,29,29,29,29,28,27,26,26,26,26,25,25,25,24,24,24,23,23,23,22,22,22,21,21,21,20,19,19,18,18,17,17,17,16,16,15,15,14,14,13,13,12,12,12,12,11,11,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,13,13,13,14,14,15,16,16,16,17,18,18,19,19,19,19,20,21,21,22,22,22,22,22,22,23,23,23,23,23,23,24,24,24,23,23,24,24,24,24,23,24,24,24,24,24,24,24,24,24,24,25,25,24,24,24,24,24,24,24,24,24,24,23,23,22,23,23,22,22,21,21,21,21,21,21,20,19,19,19,19,18,18,17,16,16,16,16,16,16,15,15,15,15,15,14,13,13,13,13,13,12,12,12,11,10,10,10,10,9,9,9,8,8,8,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,4,4,4,4,4,4,3,4,4,5,5,5,4,4,4,5,5,6,6,6,6,7,7,8,8,8,8,9,10,10,11,11,11,12,13,13,14,14,14,15,16,17,18,18,19,20,20,21,21,22,23,23,24,24,25,26,26,27,27,27,27,28,28,28,29,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,30,30,30,29,29,28,28,27,27,26,26,25,24,24,23,22,22,21,21,20,20,20,19,17,16,16,16};

const uint8_t octave[98] = {14,8,10,8,11,14,9,9,9,11,18,18,15,16,16,16,16,19,23,22,22,22,22,16,8,9,9,9,9,10,15,16,15,17,14,12,11,9,9,10,10,9,13,17,16,16,16,17,10,2,4,3,4,4,4,11,17,18,17,18,25,26,23,23,23,23,23,26,30,29,29,29,29,23,15,16,16,15,16,16,21,23,21,23,20,18,17,15,15,16,15,15,18,22,22,21,22,22};

#endif
