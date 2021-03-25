#include "game.h"

int tile_map_l1[N_BRICK_LINE][N_BRICK_COLUMN] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int tile_map_l2[N_BRICK_LINE][N_BRICK_COLUMN] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int** tile_map = NULL;

int bar_width = 175;
int bar_height = 12;
int bar_x = INTERFACE_WIDTH / 2;
int bar_y = INTERFACE_HEIGHT - 80;

int void_y = INTERFACE_HEIGHT - 65;

struct Ball* l_ball = NULL;
int n_ball = 0;

enum Bonus bonus = NONE_BONUS;
int bonus_time_remained = 0;
SDL_TimerID timer_bonus_id = NULL;

enum GameStatus game_status = NOT_STARTED;

int n_life = 3;
int score = 0;
int level_index = 1;

int time_elapsed = 0;
SDL_TimerID timer_id = NULL;
SDL_mutex* mutex_time = NULL;


void CreateGame() {

	SetTileMap(1);

	struct Ball* ball = AddBall(INTERFACE_WIDTH / 2, INTERFACE_HEIGHT / 2 + 45);
	StartBall(ball, -1);

	mutex_time = SDL_CreateMutex();
	if (!mutex_time) {
		SDL_Log("Error SDL_CreateMutex : %s", SDL_GetError());
	}

	ShowStartMessage();
}

void DestroyGame() {
	Free2DimIntArray(tile_map);

	SDL_DestroyMutex(mutex_time);
}

void StartGame() {
	game_status = IN_PROGRESS;

	StartTimer();
	HideStartMessage();
}

void ResetGame()
{
	SetTileMap(1);

	time_elapsed = 0;

	n_life = 3;
	score = 0;

	DestroyBalls();

	struct Ball* ball = AddBall(INTERFACE_WIDTH / 2, INTERFACE_HEIGHT / 2 + 45);
	StartBall(ball, -1);
}

int **Alloc2DimIntArray(int L, int C)
{
	int** array = NULL;
	array = malloc(L * sizeof(int*));

	if (array == NULL) {
		printf("Error malloc");
	}

	for (int i = 0; i < L; i++) {
		*(array + i) = (int *) malloc(C * sizeof(int));

		if (array[i] == NULL) {
			printf("Error malloc");
		}
	}

	return array;
}

void Free2DimIntArray(int **array) {

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

	if (level == 1) {
		int tile_map_size = N_BRICK_COLUMN * N_BRICK_LINE * sizeof(int);

		for (int l = 0; l < N_BRICK_LINE; l++) {
			memcpy(tile_map[l], tile_map_l1[l], tile_map_size / N_BRICK_LINE);
		}
	}
	else if (level == 2) {
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

int GetBarWidth() {
	return bar_width;
}

int GetBarHeight() {
	return bar_height;
}

int GetBarX() {
	return bar_x;
}

int GetBarY() {
	return bar_y;
}

void SetBarX(int _bar_x) {
	bar_x = _bar_x;
}

void SetBarY(int _bar_y) {
	bar_y = _bar_y;
}

struct Ball* AddBall(double x, double y) {

	int ball_array_new_size = (n_ball + 1) * sizeof(struct Ball);

	struct Ball* l_ball_tmp = NULL;
	l_ball_tmp = (struct Ball*)realloc(l_ball, ball_array_new_size);
	if (l_ball_tmp == NULL) {
		printf("Error realloc");
		ExitBreakout(EXIT_FAILURE);
	}

	l_ball = l_ball_tmp;

	struct Ball ball;

	ball.x = x;
	ball.y = y;
	ball.speed = 0.;
	ball.radius = 8;
	ball.color = RED;
	ball.direction = -1;
	ball.build_in_bricks = false;

	n_ball++;
	struct Ball* ball_ptr = l_ball + n_ball - 1;
	*ball_ptr = ball;

	return ball_ptr;

}

void RemoveBall(int index) {

	for (int i = 0; i < n_ball; i++) {

		if (i > index) {
			*(l_ball + i - 1) = *(l_ball + i);
		}
	}

	n_ball--;

	int ball_array_new_size = (n_ball) * sizeof(struct Ball);
	if (ball_array_new_size == 0) {
		return;
	}

	struct Ball* l_ball_tmp = NULL;
	l_ball_tmp = (struct Ball*)realloc(l_ball, ball_array_new_size);
	if (l_ball_tmp == NULL) {
		printf("Error realoc");
		ExitBreakout(EXIT_FAILURE);
	}

	l_ball = l_ball_tmp;
}

void StartBall(struct Ball* ball, double direction)

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

struct Ball GetBall(int index) {
	return *(l_ball + index);
}

int GetNLife()
{
	return n_life;
}

int GetScore()
{
	return score;
}

enum Bonus GetBonus()
{
	return bonus;
}

void EndBonus()
{
	if (bonus != NULL) {
		SDL_RemoveTimer(timer_bonus_id);
	}

	if (bonus == SPEED3) {

		for (int i = 0; i < n_ball; i++) {

			struct Ball* ball = l_ball + i;
			ball->speed = ball->speed_to_restore;
		}

		HideSpeed3Message();
	}

	bonus = NONE_BONUS;
}

void SetBonus(enum Bonus p_bonus) 
{
	if (bonus != NULL) {
		EndBonus();
	}

	bonus = p_bonus;

	Uint32 interval = 0;
	if (bonus == SPEED3) {
		interval = 3000;
		
		for (int i = 0; i < n_ball; i++) {

			struct Ball* ball = l_ball + i;
			ball->speed_to_restore = ball->speed;
			ball->speed = ball->speed * 3;
		}

		ShowSpeed3Message();
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

int GetNBall() {
	return n_ball;
}

void UpdateBallMovement(struct Ball* ball) {

	//printf("%d", ball->direction);

	double ball_x = ball->x;
	double ball_y = ball->y;

	if (ball->direction >= 0 && ball->direction < 90) {
		ball_x = ball->x + BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (90. - ball->direction);
		ball_y = ball->y - BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * ball->direction;
	}
	else if (ball->direction >= 90 && ball->direction < 180) {
		ball_x = ball->x - BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (ball->direction - 90.);
		ball_y = ball->y - BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (180. - ball->direction);
	}
	else if (ball->direction >= 180 && ball->direction < 270) {
		ball_x = ball->x - BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (270. - ball->direction); 
		ball_y = ball->y + BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (ball->direction - 180.); 
	}
	else if (ball->direction >= 270 && ball->direction < 360) {
		ball_x = ball->x + BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (ball->direction - 270.);
		ball_y = ball->y + BALL_MOVE_BASE_VALUE * (ball->speed / FPS) * (360. - ball->direction);
	}


	struct Circle ball_circle;
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
				
				enum BrickIntersection intersection = CircleIntersectRect(ball_circle, brick_rect);


				if (intersection != NONE_INTERSECTION) {

					if (intersection == CORNER_BOTTOM_LEFT) {

						if (ball->direction >= 0 && ball->direction < 90) {
							intersection = CORNER;
						}
						else if (ball->direction >= 90 && intersection < 180) {
							intersection = TOP_OR_BOTTOM;
						}
						else if (ball->direction >= 270 && intersection < 360) {
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
				//ball->direction = ball->direction - 90;
				ball->direction = 135;
			}
			else if (ball->direction >= 270 && ball->direction < 360) {
				//ball->direction = (ball->direction + 90) % 360;
				ball->direction = 45;
			}
		}
		else if (ball_x >= bar_part2_min_x && ball_x <= bar_part2_max_x) {

			if (ball->direction >= 180 && ball->direction < 270) {
				//ball->direction = ball->direction - 90;
				ball->direction = 157;
	
			}
			else if (ball->direction >= 270 && ball->direction < 360) {
				//ball->direction = (ball->direction + 90) % 360;
				ball->direction = 22;
			}
		}
		else if(ball_x >= bar_part3_min_x && ball_x <= bar_part3_max_x) {

			if (ball->direction >= 180 && ball->direction < 270) {

				if (ball->x <= bar_part2_min_x) {
					ball->direction = ball->direction - 90;
				}
				else if (ball->x >= bar_part2_max_x) {
					ball->direction = ball->direction - 180;
				}
			}
			else if (ball->direction >= 270 && ball->direction < 360) {

				if (ball->x <= bar_part2_min_x) {
					ball->direction = ball->direction - 180;
				}
				else if (ball->x >= bar_part2_max_x) {
					ball->direction = (ball->direction + 90) % 360;
				}
			}
		}
	}


	ball->x = ball_x;
	ball->y = ball_y;

	//printf(" %d \n", ball->direction);
}

void NextLevel()
{
	level_index++;
	SetTileMap(level_index);

	DestroyBalls();

	ShowNextlevelMessage(level_index);
}

enum BrickIntersection CircleIntersectRect(struct Circle circle, struct SDL_Rect rect)
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

void UpdateGame() {

	for (int i = 0; i < n_ball; i++) {
		struct Ball *ball = l_ball+i;

		if (game_status == IN_PROGRESS) {
			UpdateBallMovement(ball);

			if (ball->y >= void_y) {
				RemoveBall(i);
				n_life--;

				if (n_life >= 1) {
					struct Ball* ball = AddBall(INTERFACE_WIDTH / 2, INTERFACE_HEIGHT / 2);
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
}

void GameEvent(SDL_Event event)
{

	switch (event.type) {
	case SDL_KEYDOWN:
	{

		if (game_status == NOT_STARTED) {

			if (event.key.keysym.sym == SDLK_SPACE) {
				StartGame();
			}
		}
		else if (game_status == FINISHED) {

			if (event.key.keysym.sym == SDLK_SPACE) {

				HideGameoverMessage();

				ResetGame();
				StartGame();
			}
		}
		else if (game_status == IN_PROGRESS) {

			if (event.key.keysym.sym == SDLK_RIGHT) {
				if ((bar_x + bar_width / 2 + BAR_MOVE_VALUE) < INTERFACE_WIDTH) {
					bar_x += BAR_MOVE_VALUE;
				}
				else {
					bar_x = INTERFACE_WIDTH - bar_width / 2 - 1;
				}
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (bar_x - bar_width / 2 - BAR_MOVE_VALUE >= 0) {
					bar_x -= BAR_MOVE_VALUE;
				}
				else {
					bar_x = bar_width / 2;
				}
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
