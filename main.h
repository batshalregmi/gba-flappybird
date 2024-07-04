#ifndef MAIN_H
#define MAIN_H

#include "gba.h"


void showMainMenu(void);
void play(void);
void drawTube(void);
void delay(int n);
void initTimer(u16 seconds);
void updateTimer(void);
void updateScreenTimer(void);
void checkCollision(void);
struct bird {
    volatile int row;
    volatile int col;
    volatile int direction;
};


typedef struct {
    int row;
    int col;
    int height;
    int width;
} Tube;

void updatePlayer(struct bird *birdPos);

#endif
