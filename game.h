#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "math.h"

#include "interface.h"
//#include "main.c"

#define N_BRICK_COLUMN 20
#define N_BRICK_LINE 5
#define BRICK_WIDTH 45
#define BRICK_HEIGHT 30

#define BAR_MOVE_VALUE 35
#define BALL_MOVE_BASE_VALUE 7

#define BAR_PART1_RATIO 0.6
#define BAR_PART2_RATIO 0.3
#define BAR_PART3_RATIO 0.1



enum GameStatus { NOT_STARTED = 0, IN_PROGRESS = 1, PAUSED = 2, FINISHED = 3 };

struct Ball {
	double x;
	double y;
	double speed;
	int radius;
	int direction;
	SDL_Color color;

	bool build_in_bricks;

	//Bonus
	double speed_to_restore;
};

struct Circle {
	double x;
	double y;
	double radius;
};

enum Bonus {NONE_BONUS, SPEED3};

enum BrickIntersection { NONE_INTERSECTION, LEFT_OR_RIGHT, TOP_OR_BOTTOM, CORNER_TOP_RIGHT, CORNER_TOP_LEFT, CORNER_BOTTOM_RIGHT, CORNER_BOTTOM_LEFT, CORNER };


void CreateGame();
void DestroyGame();

void StartGame();

void ResetGame();
void DestroyBalls();

int **Alloc2DimIntArray(int L, int C);
void Free2DimIntArray(int ** array);

void SetTileMap(int level);

int GetTileMapCase(int x, int y);
void SetTileMapCase(int x, int y, int case_value);

int CountRemainingBricks();

int GetBarWidth();
int GetBarHeight();

int GetBarX();
int GetBarY();
void SetBarX(int _bar_x);
void SetBarY(int _bar_y);

struct Ball* AddBall(double x, double y);
void RemoveBall(int index);
void StartBall(struct Ball* ball, double direction);

int GetNBall();
struct Ball GetBall(int index);

int GetNLife();
int GetScore();

enum Bonus GetBonus();
void SetBonus(enum Bonus bonus);
void EndBonus();

int GetTimeElapsed();
char* GetFormattedTimeElapsed();

void StartTimer();
void StopTimer();
Uint32 TimerCallback(Uint32 interval, void* parameters);

void UpdateGame();
void UpdateBallMovement(struct Ball* ball);

void NextLevel();

enum BrickIntersection CircleIntersectRect(struct Circle, struct SDL_Rect);

void GameEvent(SDL_Event event);