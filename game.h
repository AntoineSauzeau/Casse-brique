#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "SDL.h"

#define N_BRICK_COLUMN 20
#define N_BRICK_LINE 5
#define BRICK_WIDTH 45
#define BRICK_HEIGHT 30

typedef struct {
	double x;
	double y;
	double speed;
	double acceleration;
	int radius;
	int direction;
	SDL_Color color;

	bool build_in_bricks;

	//Bonus
	double speed_to_restore;

} Ball;

typedef struct {
	double x;
	double y;
	double radius;
} Circle;

typedef enum { NONE_BONUS, SPEED2, SPEED3 } Bonus;


void CreateGame();
void DestroyGame();

void UpdateGame();
void GameEvent(SDL_Event* event);


// GETTERS
int GetTileMapCase(int x, int y);

int CountRemainingBricks();

int GetBarWidth();
int GetBarHeight();
int GetBarX();
int GetBarY();

int GetNBall();
Ball* GetBall(int index);

int GetNLife();
int GetScore();
int GetLevelIndex();

Bonus GetBonus();

int GetTimeElapsed();
char* GetFormattedTimeElapsed();

