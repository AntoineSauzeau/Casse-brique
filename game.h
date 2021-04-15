#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include "SDL.h"

#define N_BRICK_COLUMN 20
#define N_BRICK_LINE 5
#define BRICK_WIDTH 45
#define BRICK_HEIGHT 30

enum GameStatus { NOT_STARTED = 0, IN_PROGRESS = 1, PAUSED = 2, FINISHED = 3 };

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

enum Bonus { NONE_BONUS, SPEED3 };

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

Ball* AddBall(double x, double y);
void RemoveBall(int index);
void StartBall(Ball* ball, double direction);

int GetNBall();
Ball* GetBall(int index);

int GetNLife();
int GetScore();
int GetLevelIndex();

enum Bonus GetBonus();
void SetBonus(enum Bonus bonus);
void EndBonus();

int GetTimeElapsed();
char* GetFormattedTimeElapsed();

void StartTimer();
void StopTimer();
Uint32 TimerCallback(Uint32 interval, void* parameters);
void StartPause();
void StopPause();

void UpdateGame();
void UpdateBallMovement(Ball* ball);

void NextLevel();

enum BrickIntersection CircleIntersectRect(Circle, struct SDL_Rect);

void GameEvent(SDL_Event* event);
