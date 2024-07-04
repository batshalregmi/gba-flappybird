#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include "gba.h"

#include "images/bird.h"
#include "images/mainMenu.h"
#include "images/fbGame.h"
#include "images/gameOver.h"
#include "images/youWin.h"

int BIRDROW = 50;
int BIRDCOL = 50;
int seconds = 0;
enum gba_state
{
  START,
  PLAY,
  WIN,
  LOSE,
};

u32 previousButtons;
u32 currentButtons;
u16 timerSeconds = 0;
int timerStarted = 0;
int hasLine = 0;

void initTimer(u16 seconds)
{
  timerSeconds = seconds * 60;
}
// the game is running at 60 frames per second,
// so we can multiply the seconds by 60 to get the number of frames and then
// decrement the timer by 1 every frame

void updateTimer(void)
{
  if (timerSeconds > 0)
  {
    waitForVBlank();
    timerSeconds--;
  }
}

enum gba_state state = START;

struct bird birdPos;

int main(void)
{
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  birdPos.row = BIRDROW;
  birdPos.col = BIRDCOL;
  birdPos.direction = 0;
  drawFullScreenImageDMA(mainMenu);
  drawCenteredString(70, 40, 80, 80, "Press Start", WHITE);

  while (1)
  {
    currentButtons = BUTTONS;
    waitForVBlank();
    updateTimer();
    // start();
    switch (state)
    {
    case START:
      state = START;
      undrawImageDMA(0, 100, 20, 70, mainMenu);
      undrawImageDMA(55, 95, 30, 20, mainMenu);
      undrawImageDMA(100, 170, 20, 70, mainMenu);
      undrawImageDMA(90, 165, 30, 20, mainMenu);
      if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
      {
        state = PLAY;
        drawRectDMA(0, 0, 50, 10, BLACK);
        initTimer(10);
        timerStarted = 1;

        birdPos.col = BIRDCOL;
        birdPos.row = BIRDROW;
        hasLine = 1;
      }
      if (birdPos.direction == 1)
      {
        undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, mainMenu);
        birdPos.col += 1;
        drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
      }
      else
      {
        undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, mainMenu);
        birdPos.col -= 1;
        drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
      }
      if (birdPos.col + BIRD_HEIGHT >= 240)
      {
        birdPos.direction = 0;
      }
      else if (birdPos.col <= 0)
      {
        birdPos.direction = 1;
      }

      break;
    case PLAY:
      if (hasLine == 1)
      {
        drawFullScreenImageDMA(fbgame);
        drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
        hasLine = 0;
      }
      drawTube();
      updateScreenTimer();
      if (timerSeconds == 0)
      {
        state = LOSE;
        birdPos.col = BIRDROW;
        birdPos.row = BIRDCOL;
        undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, gameOver);
        drawFullScreenImageDMA(gameOver);
        char buffer[41];
        sprintf(buffer, "With %d seconds left!", seconds);
        drawCenteredString(90, 80, 80, 80, buffer, WHITE);
      }
      if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
      {
        state = START;
        birdPos.col = BIRDROW;
        birdPos.row = BIRDCOL;
        undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, bird);
        drawFullScreenImageDMA(mainMenu);
        drawCenteredString(70, 40, 80, 80, "Press Start", WHITE);
        undrawImageDMA(0, 100, 20, 70, mainMenu);
        undrawImageDMA(55, 95, 30, 20, mainMenu);
        undrawImageDMA(100, 170, 20, 70, mainMenu);
        undrawImageDMA(90, 165, 30, 20, mainMenu);
      }

      waitForVBlank();
      if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons))
      {

        if (birdPos.row > 0)
        {
          undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, fbgame);
          birdPos.row -= 5;
          drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
        }
      }
      if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons))
      {
        if (birdPos.row < (160 - BIRD_HEIGHT))
        {
          undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, fbgame);
          birdPos.row += 5;
          drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
        }
      }
      if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons))
      {
        if (birdPos.col > 0)
        {
          undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, fbgame);
          birdPos.col -= 5;
          drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
        }
      }
      if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons))
      {
        if (birdPos.col < (240 - BIRD_WIDTH))
        {
          undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, fbgame);
          birdPos.col += 5;
          drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
        }
      }
      checkCollision();
      break;
    case WIN:
      if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
      {
        state = START;
        timerStarted = 0;
        birdPos.col = BIRDROW;
        birdPos.row = BIRDCOL;
        undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, bird);
        drawFullScreenImageDMA(mainMenu);
        drawCenteredString(70, 40, 80, 80, "Press Start", WHITE);
      }
      break;
    case LOSE:
      if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons))
      {
        state = START;
        timerStarted = 0;
        birdPos.col = BIRDROW;
        birdPos.row = BIRDCOL;
        undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, bird);
        drawFullScreenImageDMA(mainMenu);
        drawCenteredString(70, 40, 80, 80, "Press Start", WHITE);
      }
      break;
    }
    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}

void checkCollision(void)
{
  Tube tubes[] = {
      {0, 100, 20, 70},   // green
      {55, 95, 30, 20},   // green
      {100, 170, 20, 70}, // green
      {90, 165, 30, 20}}; // red (win)
  if ((birdPos.row < tubes[0].width + tubes[0].row && birdPos.col + BIRD_WIDTH > tubes[0].col && birdPos.col < tubes[0].col + tubes[0].height) || (birdPos.row < tubes[1].width + tubes[1].row && birdPos.col + BIRD_WIDTH > tubes[1].col && birdPos.col < tubes[1].col + tubes[1].height))
  {
    state = LOSE;
    birdPos.col = BIRDROW;
    birdPos.row = BIRDCOL;
    undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, gameOver);
    drawFullScreenImageDMA(gameOver);
    char buffer[41];
    sprintf(buffer, "With %d seconds left!", seconds);
    drawCenteredString(90, 80, 80, 80, buffer, WHITE);
  }
  // check for the lower tube
  if ((birdPos.row + BIRD_HEIGHT > tubes[2].row && birdPos.col + BIRD_WIDTH > tubes[2].col && birdPos.col < tubes[2].col + tubes[2].height) || (birdPos.row + BIRD_HEIGHT > tubes[3].row && birdPos.col + BIRD_WIDTH > tubes[3].col && birdPos.col < tubes[3].col + tubes[3].height))
  {
    state = LOSE;
    birdPos.col = BIRDROW;
    birdPos.row = BIRDCOL;
    undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, gameOver);
    drawFullScreenImageDMA(gameOver);
    char buffer[41];
    sprintf(buffer, "With %d seconds left!", seconds);
    drawCenteredString(90, 80, 80, 80, buffer, WHITE);
  }
  // if it touched the red wall on the right
  if (birdPos.col + BIRD_WIDTH > 220)
  {
    state = WIN;
    birdPos.col = BIRDROW;
    birdPos.row = BIRDCOL;
    undrawImageDMA(BIRDROW, BIRDCOL, 20, 20, youWin);
    drawFullScreenImageDMA(youWin);
    char buffer[41];
    sprintf(buffer, "With %d seconds left!", seconds);
    drawCenteredString(90, 80, 80, 80, buffer, WHITE);
  }
  // check collisoin with the timer {0, 0, 50, 10} and stop the bird from reaching the rectangle
  if (birdPos.row + BIRD_HEIGHT > 0 && birdPos.row < 10 && birdPos.col + BIRD_WIDTH > 0 && birdPos.col < 50)
  {
    undrawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, fbgame);
    birdPos.row = 50;
    birdPos.col = 50;
    drawImageDMA(birdPos.row, birdPos.col, BIRD_HEIGHT, BIRD_WIDTH, bird);
  }
}

void updateScreenTimer(void)
{
  char buffer[41];
  int time = timerSeconds / 60;
  seconds = time;
  sprintf(buffer, "Time: %d", time);
  drawRectDMA(0, 0, 50, 10, BLACK);
  drawString(0, 0, buffer, WHITE);
}

void drawTube(void)
{
  drawRectDMA(0, 100, 20, 70, GREEN);
  drawRectDMA(55, 95, 30, 20, GREEN);
  drawRectDMA(100, 170, 20, 70, GREEN);
  drawRectDMA(90, 165, 30, 20, GREEN);
  drawRectDMA(0, 220, 20, 220, RED);
}
