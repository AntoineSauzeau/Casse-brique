#include "game.h"

#include <math.h>
#include <errno.h>
#include <stdio.h>

#include "interface_game.h"
#include "interface.h"
#include "colors.h"

/*int tile_map_l1[N_BRICK_LINE][N_BRICK_COLUMN] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};*/

int tile_map_l1[N_BRICK_LINE][N_BRICK_COLUMN] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 2, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 2, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int tile_map_l2[N_BRICK_LINE][N_BRICK_COLUMN] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 5, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 5, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

#define BAR_MOVE_VALUE 35
#define BALL_MOVE_BASE_VALUE 7

#define BAR_PART1_RATIO 0.4
#define BAR_PART2_RATIO 0.5
#define BAR_PART3_RATIO 0.1

#define BALL_SPAWN_X (INTERFACE_WIDTH / 2)
#define BALL_SPAWN_Y (INTERFACE_HEIGHT / 2 + 45)

#define N_LEVEL 2

typedef enum { NONE_INTERSECTION, LEFT_OR_RIGHT, TOP_OR_BOTTOM, CORNER_TOP_RIGHT, CORNER_TOP_LEFT, CORNER_BOTTOM_RIGHT, CORNER_BOTTOM_LEFT, CORNER } BrickIntersection;
typedef enum { NOT_STARTED = 0, IN_PROGRESS = 1, PAUSED = 2, FINISHED = 3 } GameStatus;


static int** tile_map = NULL;

static int bar_width = 175;
static int bar_height = 12;
static int bar_x = INTERFACE_WIDTH / 2;
static int bar_y = INTERFACE_HEIGHT - 80;

static int void_y = INTERFACE_HEIGHT - 65;

static Ball* l_ball = NULL;
static int n_ball = 0;

static Bonus bonus = NONE_BONUS;
static int bonus_time_remained = 0;
static SDL_TimerID timer_bonus_id = NULL;

static GameStatus game_status = NOT_STARTED;

static int n_life = 3;
static int score = 0;
static int level_index = 0;

static int time_elapsed = 0;
static SDL_TimerID timer_id = NULL;
static SDL_mutex* mutex_time = NULL;

static bool add_ball = false;



/*
*	Prototypes.
*/
static void StartGame();

static void ResetGame();
static void DestroyBalls();

static void UpdateBallMovement(Ball* ball);
static BrickIntersection CircleIntersectRect(Circle, struct SDL_Rect);

static void NextLevel();

static void StartTimer();
static void StopTimer();
static Uint32 TimerCallback(Uint32 interval, void* parameters);

static void StartPause();
static void StopPause();

static void SetTileMap(int level);
static void SetTileMapCase(int x, int y, int case_value);

static void SetBonus(Bonus bonus);
static void EndBonus();

static Ball* AddBall(double x, double y);
static void RemoveBall(int index);
static void StartBall(Ball* ball, double direction);

static int** Alloc2DimIntArray(int L, int C);
static void Free2DimIntArray(int** array);




void CreateGame() 
{

	SetTileMap(1);

	Ball* ball = AddBall(INTERFACE_WIDTH / 2, INTERFACE_HEIGHT / 2 + 45);
	StartBall(ball, -1);

	mutex_time = SDL_CreateMutex();
	if (!mutex_time) {
		SDL_Log("Error SDL_CreateMutex : %s", SDL_GetError());
	}

	ShowStartMessage();
}

void DestroyGame() 
{
	Free2DimIntArray(tile_map);

	SDL_DestroyMutex(mutex_time);
}

void StartGame() 
{
	game_status = IN_PROGRESS;

	StartTimer();
	HideStartMessage();
}

void ResetGame()
{
	level_index = 0;
	SetTileMap(level_index);

	time_elapsed = 0;

	n_life = 3;
	score = 0;

	DestroyBalls();

	Ball* ball = AddBall(BALL_SPAWN_X, BALL_SPAWN_Y);
	StartBall(ball, -1);
}

int **Alloc2DimIntArray(int L, int C)
{
	errno = 0;

	int** array = NULL;
	array = malloc(L * sizeof(int*));

	if (array == NULL) {
		ShowError("Error malloc : %s", errno);

		return NULL;
	}

	for (int i = 0; i < L; i++) {
		*(array + i) = (int *) malloc(C * sizeof(int));

		if (array[i] == NULL) {
			ShowError("Error malloc : %s", errno);
		}
	}

	return array;
}

void Free2DimIntArray(int **array) 
{

	int size_array = sizeof(array);

	int n_column = sizeof(array[0]);
	int n_line = size_array / n_column;

	for (int l = 0; l < n_line; l++) {
		free(array[l]);
	}
}

void DestroyBalls()
{
	for (int i = 0; i < n_ball; i++) {
		RemoveBall(i);
	}
}

void SetTileMap(int level)
{
	
	if (tile_map == NULL) {
		tile_map = Alloc2DimIntArray(N_BRICK_LINE, N_BRICK_COLUMN);
	}

	if (level == 0) {
		int tile_map_size = N_BRICK_COLUMN * N_BRICK_LINE * sizeof(int);

		for (int l = 0; l < N_BRICK_LINE; l++) {
			memcpy(tile_map[l], tile_map_l1[l], tile_map_size / N_BRICK_LINE);
		}
	}
	else if (level == 1) {
		int tile_map_size = N_BRICK_COLUMN * N_BRICK_LINE * sizeof(int);

		for (int l = 0; l < N_BRICK_LINE; l++) {
			memcpy(tile_map[l], tile_map_l2[l], tile_map_size / N_BRICK_LINE);
		}
	}

}

int GetTileMapCase(int x, int y)
{
	return *(*(tile_map + y) + x);
}

void SetTileMapCase(int x, int y, int case_value)
{
	tile_map[y][x] = case_value;
}

int CountRemainingBricks()
{
	int n_brick_remaining = 0;
	for (int l = 0; l < N_BRICK_LINE; l++) {

		for (int c = 0; c < N_BRICK_COLUMN; c++) {

			int brick_value = *(*(tile_map + l) + c);
			if (brick_value != 0) {
				n_brick_remaining++;
			}
		}
	}

	return n_brick_remaining;
}

int GetBarWidth() 
{
	return bar_width;
}

int GetBarHeight() 
{
	return bar_height;
}

int GetBarX()
{
	return bar_x;
}

int GetBarY() 
{
	return bar_y;
}

Ball* AddBall(double x, double y) 
{

	errno = 0;

	int ball_array_new_size = (n_ball + 1) * sizeof(Ball);

	if (n_ball != 0) {
		Ball* l_ball_tmp = NULL;
		l_ball_tmp = (Ball*)realloc(l_ball, ball_array_new_size);
		if (l_ball_tmp == NULL) {

			ShowError("Error realloc : %s", errno);

			free(l_ball);
			ExitBreakout(EXIT_FAILURE);
		}

		l_ball = l_ball_tmp;
	}
	else {
		l_ball = malloc(ball_array_new_size);
	}

	Ball ball;

	ball.x = x;
	ball.y = y;
	ball.speed = 0.;
	ball.speed_to_restore = -1;
	ball.acceleration = 0.;
	ball.radius = 8;
	ball.color = RED;
	ball.direction = -1;
	ball.build_in_bricks = false;

	n_ball++;
	Ball* ball_ptr = l_ball + n_ball - 1;
	if (ball_ptr != NULL) {
		*ball_ptr = ball;
	}

	return ball_ptr;

}

void RemoveBall(int index) 
{

	errno = 0;

	for (int i = 0; i < n_ball; i++) {

		if (i > index) {
			*(l_ball + i - 1) = *(l_ball + i);
		}
	}

	n_ball--;

	int ball_array_new_size = (n_ball) * sizeof(Ball);
	if (ball_array_new_size == 0) {

		free(l_ball);
		return;
	}

	Ball* l_ball_tmp = NULL;
	l_ball_tmp = (Ball*)realloc(l_ball, ball_array_new_size);
	if (l_ball_tmp == NULL) {

		ShowError("Error realloc : %s", errno);

		free(l_ball);
		ExitBreakout(EXIT_FAILURE);
	}

	l_ball = l_ball_tmp;
}

void StartBall(Ball* ball, double direction)
{
	if (direction == -1) {

		time_t t;
		srand((unsigned)time(&t));

		ball->direction = 60 + rand() % 80;
		printf("%d", ball->direction);
	}
	else {
		ball->direction = direction;
	}

	ball->speed = 1.0;
}

Ball* GetBall(int index) 
{
	return l_ball + index;
}

int GetNLife()
{
	return n_life;
}

int GetScore()
{
	return score;
}

Bonus GetBonus()
{
	return bonus;
}

void EndBonus()
{
	if (bonus != NULL) {
		SDL_RemoveTimer(timer_bonus_id);
	}

	if (bonus == SPEED2 || bonus == SPEED3) {

		for (int i = 0; i < n_ball; i++) {

			Ball* ball = l_ball + i;

			if (ball->speed_to_restore != -1) {
				ball->speed = ball->speed_to_restore;
				ball->speed_to_restore = -1;
			}
		}
	}

	bonus = NONE_BONUS;
}

void SetBonus(Bonus p_bonus) 
{
	if (bonus != NULL) {
		EndBonus();
	}

	bonus = p_bonus;

	Uint32 interval = 0;
	if (bonus == SPEED2 || bonus == SPEED3) {
		
		for (int i = 0; i < n_ball; i++) {

			Ball* ball = l_ball + i;
			ball->speed_to_restore = ball->speed;

			if (bonus == SPEED2) {
				ball->speed = ball->speed * 2;
			}
			else if (bonus == SPEED3) {
				ball->speed = ball->speed * 3;
			}
		}

		if (bonus == SPEED2) {
			interval = 5000;
			ShowSpeed2Message();
		}
		else if (bonus == SPEED3) {
			interval = 3000;
			ShowSpeed3Message();
		}
	}
	
	if (interval != 0) {

		timer_bonus_id = SDL_AddTimer(interval, EndBonus, NULL);
		if (!timer_bonus_id) {
			SDL_Log("Error SDL_AddTimer (Bonus) : %s", SDL_GetError());
		}
	}
}

int GetTimeElapsed()
{
	if (SDL_LockMutex(mutex_time) == 0) {

		SDL_UnlockMutex(mutex_time);
		return time_elapsed;
	}
	else {
		SDL_Log("Error SDL_LockMutex : %s", SDL_GetError());
		return 0;
	}
}

char* GetFormattedTimeElapsed()
{
	int s_time = 0;
	if (SDL_LockMutex(mutex_time) == 0) {

		SDL_UnlockMutex(mutex_time);
		s_time = time_elapsed;
	}
	else {
		SDL_Log("Error SDL_LockMutex : %s", SDL_GetError());
	}

	int n_hour = 0, n_minute = 0, n_second = 0;

	if (s_time >= 3600) {
		n_hour = s_time / 3600;
		s_time -= n_hour * 3600;
	}
	if (s_time >= 60) {
		n_minute = s_time / 60;
		s_time -= n_minute * 60;
	}
	n_second = s_time;

	char hour_string[3];
	sprintf_s(hour_string, 3, "%d", n_hour);

	char minute_string[3];
	sprintf_s(minute_string, 3, "%d", n_minute);

	char second_string[3];
	sprintf_s(second_string, 3, "%d", n_second);

	char time_elapsed_string[9] = { "" };
	if (n_hour < 10) {
		strcat_s(time_elapsed_string, 9 ,"0");
	}
	strcat_s(time_elapsed_string, 9, hour_string);
	strcat_s(time_elapsed_string, 9, ":");

	if (n_minute < 10) {
		strcat_s(time_elapsed_string, 9, "0");
	}
	strcat_s(time_elapsed_string, 9, minute_string);
	strcat_s(time_elapsed_string, 9, ":");

	if (n_second < 10) {
		strcat_s(time_elapsed_string, 9, "0");
	}
	strcat_s(time_elapsed_string, 9, second_string);

	return time_elapsed_string;
}

int GetLevelIndex()
{
	return level_index;
}

void StartTimer() 
{
	if (timer_id == NULL) {

		timer_id = SDL_AddTimer(1000, TimerCallback, NULL);
		if (timer_id == 0) {
			SDL_Log("Error SDL_AddTimer : %s", SDL_GetError());
		}

	}
}

void StopTimer()
{
	SDL_RemoveTimer(timer_id);
	timer_id = NULL;
}

Uint32 TimerCallback(Uint32 interval, void* parameters)
{
	if (SDL_LockMutex(mutex_time) == 0) {
		time_elapsed += 1;
		SDL_UnlockMutex(mutex_time);
	}
	else {
		SDL_Log("Error SDL_LockMutex : %s", SDL_GetError());
	}

	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return interval;
}

int GetNBall() 
{
	return n_ball;
}

void UpdateBallMovement(Ball* ball) 
{

	double ball_x = ball->x;
	double ball_y = ball->y;

	double speed = ball->speed;
	if (ball->acceleration != 0.) {
		speed = speed * ball->acceleration;
	}


	if (ball->direction >= 0 && ball->direction < 90) {
		ball_x = ball->x + BALL_MOVE_BASE_VALUE * (speed / FPS) * (90. - ball->direction);
		ball_y = ball->y - BALL_MOVE_BASE_VALUE * (speed / FPS) * ball->direction;
	}
	else if (ball->direction >= 90 && ball->direction < 180) {
		ball_x = ball->x - BALL_MOVE_BASE_VALUE * (speed / FPS) * (ball->direction - 90.);
		ball_y = ball->y - BALL_MOVE_BASE_VALUE * (speed / FPS) * (180. - ball->direction);
	}
	else if (ball->direction >= 180 && ball->direction < 270) {
		ball_x = ball->x - BALL_MOVE_BASE_VALUE * (speed / FPS) * (270. - ball->direction); 
		ball_y = ball->y + BALL_MOVE_BASE_VALUE * (speed / FPS) * (ball->direction - 180.); 
	}
	else if (ball->direction >= 270 && ball->direction < 360) {
		ball_x = ball->x + BALL_MOVE_BASE_VALUE * (speed / FPS) * (ball->direction - 270.);
		ball_y = ball->y + BALL_MOVE_BASE_VALUE * (speed / FPS) * (360. - ball->direction);
	}


	Circle ball_circle;
	ball_circle.x = ball->x;
	ball_circle.y = ball->y;
	ball_circle.radius = ball->radius;

	//Rebond mur
	if ((ball_x - (double)ball->radius) < 0) {
		ball_x = 0 + ball->radius;

		if (ball->direction >= 90 && ball->direction < 180) {
			ball->direction = 90 - (ball->direction % 90);
		}
		else if (ball->direction >= 180 && ball->direction < 270) {
			ball->direction = 360 - (ball->direction % 90);
		}
	}
	else if ((ball_x + ball->radius) >= INTERFACE_WIDTH) {
		ball_x = (double)INTERFACE_WIDTH - ball->radius;

		if (ball->direction >= 0 && ball->direction < 90) {
			ball->direction = 180 - (ball->direction % 90);
		}
		else if (ball->direction >= 270 && ball->direction < 360) {
			ball->direction = 270 - (ball->direction % 90);
		}
	}

	if ((ball_y - (double)ball->radius) < BRICK_HEIGHT) {
		ball_y = (double)BRICK_HEIGHT + ball->radius;

		if (ball->direction >= 0 && ball->direction < 90) {
			ball->direction = 360 - (ball->direction % 90);
		}
		else if (ball->direction >= 90 && ball->direction < 180) {
			ball->direction = 270 - (ball->direction % 90);
		}
	}
	else if ((ball_y + ball->radius) >= INTERFACE_HEIGHT) {
		ball_y = (double)INTERFACE_HEIGHT - ball->radius;

		if (ball->direction >= 180 && ball->direction < 270) {
			ball->direction = 180 - (ball->direction % 90);
		}
		else if (ball->direction >= 270 && ball->direction < 360) {
			ball->direction = 90 - (ball->direction % 90);
		}
	}



	//Rebond brique
	double initial_ball_direction = ball->direction;
	int ball_direction_to_keep = -1;
	for (int l = 0; l < N_BRICK_LINE; l++) {

		for (int c = 0; c < N_BRICK_COLUMN; c++) {

			int brick_value = GetTileMapCase(c, l);
			if (brick_value != 0) {

				double brick_center_x = (double)c * BRICK_WIDTH + BRICK_WIDTH / 2;
				double brick_center_y = (double)l * BRICK_HEIGHT + BRICK_HEIGHT / 2 + (double)2 * BRICK_HEIGHT;

				SDL_Rect brick_rect = { brick_center_x, brick_center_y, BRICK_WIDTH , BRICK_HEIGHT };
				
				BrickIntersection intersection = CircleIntersectRect(ball_circle, brick_rect);


				if (intersection != NONE_INTERSECTION) {

					if (intersection == CORNER_BOTTOM_LEFT) {

						if (ball->direction >= 0 && ball->direction < 90) {
							intersection = CORNER;
						}
						else if (ball->direction >= 90 && ball->direction >= 90 < 180) {
							intersection = TOP_OR_BOTTOM;
						}
						else if (ball->direction >= 270 && ball->direction >= 90 < 360) {
							intersection = LEFT_OR_RIGHT;
						}
					}
					else if (intersection == CORNER_BOTTOM_RIGHT) {

						if (ball->direction >= 90 && ball->direction < 180) {
							intersection = CORNER;
						}
						else if (ball->direction >= 0 && ball->direction < 90) {
							intersection = TOP_OR_BOTTOM;
						}
						else if (ball->direction >= 180 && ball->direction < 270) {
							intersection = LEFT_OR_RIGHT;
						}
					}
					else if (intersection == CORNER_TOP_LEFT) {

						if (ball->direction >= 270 && ball->direction < 360) {
							intersection = CORNER;
						}
						else if (ball->direction >= 180 && ball->direction < 270) {
							intersection = TOP_OR_BOTTOM;
						}
						else if (ball->direction >= 0 && ball->direction < 90) {
							intersection = LEFT_OR_RIGHT;
						}
					}
					else if (intersection == CORNER_TOP_RIGHT) {

						if (ball->direction >= 180 && ball->direction < 270) {
							intersection = CORNER;
						}
						else if (ball->direction >= 270 && ball->direction < 360) {
							intersection = TOP_OR_BOTTOM;
						}
						else if (ball->direction >= 90 && ball->direction < 180) {
							intersection = LEFT_OR_RIGHT;
						}
					}

					if (intersection == LEFT_OR_RIGHT) {
						printf("LEFT_OR_RIGHT \n");

						//Left
						if (ball->direction >= 0 && ball->direction < 90) {
							ball->direction = 180 - (ball->direction % 90);
						}
						else if (ball->direction >= 270 && ball->direction < 360) {
							ball->direction = 270 - (ball->direction % 90);
						}

						//Right
						else if (ball->direction >= 90 && ball->direction < 180) {
							ball->direction = 90 - (ball->direction % 90);
						}
						else if (ball->direction >= 180 && ball->direction < 270) {
							ball->direction = 360 - (ball->direction % 90);
						}

					}
					else if (intersection == TOP_OR_BOTTOM) {
						printf("TOP_OR_BOTTOM \n");

						//Bottom
						if (ball->direction >= 0 && ball->direction < 90) {
							ball->direction = 360 - (ball->direction % 90);
						}
						else if (ball->direction >= 90 && ball->direction < 180) {
							ball->direction = 270 - (ball->direction % 90);
						}

						//Top
						else if (ball->direction >= 180 && ball->direction < 270) {
							ball->direction = 180 - (ball->direction % 90);
						}
						else if (ball->direction >= 270 && ball->direction < 360) {
							ball->direction = 90 - (ball->direction % 90);
						}

					}
					else if (intersection == CORNER) {
						printf("CORNER \n");

						ball->direction = (ball->direction + 180) % 360;
					}

					int brick = GetTileMapCase(c, l);
					if (brick == 1) {
						score += 20;
					}
					else if (brick == 2) {
						score += 50;

						SetBonus(SPEED3);
						timer_bonus_id = SDL_AddTimer(3000, EndBonus, NULL);
					}
					else if (brick == 3) {
						score += 50;

						n_life++;
					}
					else if (brick == 4) {
						score += 50;

						ShowNewballMessage();
						add_ball = true;
					}
					else if (brick == 5) {
						score += 50;

						SetBonus(SPEED2);
						timer_bonus_id = SDL_AddTimer(5000, EndBonus, NULL);
					}

					if (brick != 0) {
						ball->acceleration = 0.;
					}


					SetTileMapCase(c, l, 0);
				}
			}

			//If some bricks are affected, the direction is changed only once
			if (initial_ball_direction != ball->direction) {
				
				if (ball_direction_to_keep == -1) {

					printf("New direction %f -> ", initial_ball_direction);
					printf("%d \n", ball->direction);

					ball_direction_to_keep = ball->direction;
				}
			}
		}
	}

	if (ball_direction_to_keep != -1) {
		ball->direction = ball_direction_to_keep;
	}


	//Rebond bar
	SDL_Rect bar_rect;
	bar_rect.x = bar_x;
	bar_rect.y = bar_y;
	bar_rect.w = bar_width;
	bar_rect.h = bar_height;

	if (CircleIntersectRect(ball_circle, bar_rect)) {
		
		double bar_part1_width = bar_width * BAR_PART1_RATIO;
		double bar_part2_width = bar_width * BAR_PART2_RATIO;
		double bar_part3_width = bar_width * BAR_PART3_RATIO;

		double bar_part1_min_x = bar_x - bar_part1_width / 2;
		double bar_part1_max_x = bar_x + bar_part1_width / 2;
		
		double bar_part2_min_x = bar_part1_min_x - bar_part2_width / 2;
		double bar_part2_max_x = bar_part1_max_x + bar_part2_width / 2;

		double bar_part3_min_x = bar_part2_min_x - bar_part3_width / 2;
		double bar_part3_max_x = bar_part2_max_x + bar_part3_width / 2;

		if (ball_x >= bar_part1_min_x && ball_x <= bar_part1_max_x) {

			if (ball->direction >= 180 && ball->direction < 270) {
				ball->direction = 110;
			}
			else if (ball->direction >= 270 && ball->direction < 360) {
				ball->direction = 70;
			}
		}
		else if (ball_x >= bar_part2_min_x && ball_x <= bar_part2_max_x) {

			if (ball_x >= bar_x) {
				ball->direction = 55;
	
			}
			else if (ball_x < bar_x) {
				ball->direction = 125;
			}
		}
		else if(ball_x >= bar_part3_min_x && ball_x <= bar_part3_max_x) {

			if (ball_x >= bar_x) {
				ball->direction = 40;
			}
			else if (ball_x < bar_x) {
				ball->direction = 140;
			}

			ball->acceleration = 1.6;
		}
	}

	/*printf("x: %f - %f \n", ball_x, ball->x);
	printf("y: %f - %f \n", ball_y, ball->y);
	printf("speed : %f \n", speed);*/

	ball->x = ball_x;
	ball->y = ball_y;
}

void NextLevel()
{
	level_index++;
	SetTileMap(level_index);

	DestroyBalls();
	Ball* ball = AddBall(BALL_SPAWN_X, BALL_SPAWN_Y);
	StartBall(ball, -1);

	Audio_PlayMusic(1, -1);
	ShowNextlevelMessage(level_index);
}

BrickIntersection CircleIntersectRect(Circle circle, struct SDL_Rect rect)
{

	double distance_x = fabs(circle.x - rect.x);
	double distance_y = fabs(circle.y - rect.y);

	if (distance_x > (rect.w / 2 + circle.radius)) {
		return NONE_INTERSECTION;
	}
	if (distance_y > (rect.h / 2 + circle.radius)) {
		return NONE_INTERSECTION;
	}

	if (distance_x <= rect.w / 2 || distance_y <= rect.h / 2) {

		if ((distance_x - rect.w / 2) > (distance_y - rect.h / 2)) {
			return LEFT_OR_RIGHT;
		}
		else if ((distance_x - rect.w / 2) < (distance_y - rect.h / 2)){
			return TOP_OR_BOTTOM;
		}

	}

	double corner_distance = pow((distance_x - rect.w / 2), 2) + pow((distance_y - rect.h / 2), 2);

	if ((corner_distance <= (pow(circle.radius, 2)))) {
		
		if ((circle.x < rect.x - (double)rect.w / 2) && (circle.y < rect.y - (double)rect.h / 2)) {
			return CORNER_TOP_LEFT;
		}
		else if ((circle.x > rect.x + (double)rect.w / 2) && (circle.y > rect.y + (double)rect.h / 2)) {
			return CORNER_BOTTOM_RIGHT;
		}
		else if ((circle.x < rect.x - (double)rect.w / 2) && (circle.y > rect.y + (double)rect.h / 2)) {
			return CORNER_BOTTOM_LEFT;
		}
		else if ((circle.x > rect.x + (double)rect.w / 2) && (circle.y < rect.y - (double)rect.h / 2)) {
			return CORNER_TOP_RIGHT;
		}
	}
	else {
		return NONE_INTERSECTION;
	}
	
}

void UpdateGame() 
{

	for (int i = 0; i < n_ball; i++) {
		Ball *ball = l_ball+i;

		if (game_status == IN_PROGRESS) {
			UpdateBallMovement(ball);

			if (ball->y >= void_y) {
				RemoveBall(i);
				n_life--;

				printf("Vie en moins");

				if (n_life >= 1) {
					Ball* ball = AddBall(INTERFACE_WIDTH / 2, INTERFACE_HEIGHT / 2);
					StartBall(ball, -1);
				}
				else {
					ShowGameoverMessage();

					game_status = FINISHED;
					StopTimer();
				}
			}


			if (CountRemainingBricks() == 0) {
				NextLevel();
			}
		}
	}

	if (add_ball) {
		Ball* ball = AddBall(BALL_SPAWN_X, BALL_SPAWN_Y);
		StartBall(ball, -1);

		add_ball = false;
	}
}

void GameEvent(SDL_Event* event)
{

	switch (event->type) {
	case SDL_KEYDOWN:
	{

		if (game_status == NOT_STARTED) {

			if (event->key.keysym.sym == SDLK_SPACE) {
				StartGame();
			}
		}
		else if (game_status == FINISHED) {

			if (event->key.keysym.sym == SDLK_SPACE) {

				HideGameoverMessage();

				ResetGame();
				StartGame();
			}
		}
		else if (game_status == IN_PROGRESS) {

			if (event->key.keysym.sym == SDLK_RIGHT) {
				if ((bar_x + bar_width / 2 + BAR_MOVE_VALUE) < INTERFACE_WIDTH) {
					bar_x += BAR_MOVE_VALUE;
				}
				else {
					bar_x = INTERFACE_WIDTH - bar_width / 2 - 1;
				}
			}
			else if (event->key.keysym.sym == SDLK_LEFT) {
				if (bar_x - bar_width / 2 - BAR_MOVE_VALUE >= 0) {
					bar_x -= BAR_MOVE_VALUE;
				}
				else {
					bar_x = bar_width / 2;
				}
			}
			else if (event->key.keysym.sym == SDLK_p) {
				StartPause();
			}
		}
		else if (game_status == PAUSED) {

			if (event->key.keysym.sym == SDLK_p) {
				StopPause();
			}
		}

		if (game_status == IN_PROGRESS || game_status == PAUSED) {

			if (event->key.keysym.sym == SDLK_n) {
				if ((level_index + 1) < N_LEVEL) {
					NextLevel();
				}
			}
			else if (event->key.keysym.sym == SDLK_r) {
				ResetGame();
			}
		}

		break;
	}
	case SDL_MOUSEMOTION:
	{
		int mouse_x = 0;
		int mouse_y = 0;

		SDL_GetMouseState(&mouse_x, &mouse_y);

		if (game_status == IN_PROGRESS) {

			if (mouse_x - bar_width / 2 >= 0 && mouse_x + bar_width / 2 < INTERFACE_WIDTH) {
				bar_x = mouse_x;
			}
			else if (mouse_x - bar_width / 2 < 0) {
				bar_x = bar_width / 2;
			}
			else if (mouse_x + bar_width / 2 >= INTERFACE_WIDTH) {
				bar_x = INTERFACE_WIDTH - bar_width / 2 - 1;
			}
		}

		break;
	}
	}
}

void StartPause()
{
	game_status = PAUSED;
	ShowPauseMessage();

}

void StopPause()
{
	game_status = IN_PROGRESS;
	HidePauseMessage();
}

