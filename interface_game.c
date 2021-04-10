#include "interface_game.h"

#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "interface.h"
#include "colors.h"
#include "message.h"

static SDL_Renderer* renderer = NULL;

static SDL_Thread* display_thread = NULL;

static TTF_Font* font_roboto_35 = NULL;
static TTF_Font* font_roboto_25 = NULL;
static TTF_Font* font_roboto_18 = NULL;

static SDL_Surface* image_hearth_surface = NULL;
static SDL_Texture* image_hearth_texture = NULL;

static SDL_Surface* image_rocket_surface = NULL;
static SDL_Texture* image_rocket_texture = NULL;

static SDL_Surface* image_pause_surface = NULL;
static SDL_Texture* image_pause_texture = NULL;

static Message message_start;
static Message message_speed3;
static Message message_gameover;
static Message message_nextlevel;
static Message message_newball;
static Message message_pause;

static MessageImage m_image_rocket;
static MessageImage m_image_pause;

static SDL_TimerID timer_message_nextlevel;
static SDL_TimerID timer_message_newball;


/* 
	Prototypes.
*/

static void LoadFonts();
static void FreeFonts();

static void LoadImages();
static void FreeImages();

static void CreateMessages();

static void GameLoop();



void CreateGameInterface(SDL_Renderer* _renderer) {

	renderer = _renderer;

	LoadImages();
	LoadFonts();

	InitMessages();
	CreateMessages();

	CreateGame();
}

void DestroyGameInterface() {

	DestroyMessages();

	FreeImages();
	FreeFonts();
}

void LoadFonts() {

	font_roboto_25 = TTF_OpenFont("Roboto-Medium.ttf", 25);

	if (!font_roboto_25) {
		SDL_Log("Error TTF_OpenFont : %s", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	font_roboto_18 = TTF_OpenFont("Roboto-Medium.ttf", 18);

	if (!font_roboto_18) {
		SDL_Log("Error TTF_OpenFont : %s", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	font_roboto_35 = TTF_OpenFont("Roboto-Medium.ttf", 35);

	if (!font_roboto_35) {
		SDL_Log("Error TTF_OpenFont : %s", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}
}

void FreeFonts() {
	TTF_CloseFont(font_roboto_25);
}

void LoadImages()
{
	image_hearth_surface = IMG_Load("hearth.png");
	if (!image_hearth_surface) {
		SDL_Log("Error IMG_Load : %s", IMG_GetError());
	}

	image_hearth_texture = SDL_CreateTextureFromSurface(renderer, image_hearth_surface);

	image_rocket_surface = IMG_Load("rocket.png");
	if (!image_rocket_surface) {
		SDL_Log("Error IMG_Load : %s", IMG_GetError());
	}

	image_rocket_texture = SDL_CreateTextureFromSurface(renderer, image_rocket_surface);

	image_pause_surface = IMG_Load("pause_2.png");
	if (!image_pause_surface) {
		SDL_Log("Error IMG_Load : %s", IMG_GetError());
	}

	image_pause_texture = SDL_CreateTextureFromSurface(renderer, image_pause_surface);
}

void FreeImages() {
	SDL_FreeSurface(image_hearth_surface);
	SDL_DestroyTexture(image_hearth_texture);
}

void StartGameLoop() {

	display_thread = SDL_CreateThread(GameLoop, "DisplayThread", NULL);
	if (display_thread == NULL) {
		printf("Erreur lors du lancement du thread d'affichage");
		
		ExitBreakout(EXIT_FAILURE);
	}

}

void GameLoop() {

	clock_t start_t, end_t, total_t;

	while (1) {

		start_t = clock();

		UpdateGame();
		DrawGameInterface();

		end_t = clock();
		total_t = ((double)end_t - (double)start_t) / CLOCKS_PER_SEC;

		double time_to_wait = 1. / FPS - total_t;
		time_to_wait = time_to_wait * 1000;

		if (time_to_wait < 0) {
			continue;
		}
		else {
			SDL_Delay(time_to_wait);
		}

	}
}

void DrawGameInterface() {

	SDL_SetRenderDrawColor(renderer, GREY.r, GREY.g, GREY.b, GREY.a);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, VIOLET.r, VIOLET.g, VIOLET.b, VIOLET.a);

	SDL_Rect background_2_rect = { 0, BRICK_HEIGHT, INTERFACE_WIDTH, INTERFACE_HEIGHT - 75 };
	SDL_RenderFillRect(renderer, &background_2_rect);


	SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
	SDL_RenderDrawLine(renderer, 0, BRICK_HEIGHT + 1, INTERFACE_WIDTH, BRICK_HEIGHT + 2);

	//DRAW BRICKS
	for (int l = 0; l < N_BRICK_LINE; l++) {

		for (int c = 0; c < N_BRICK_COLUMN; c++) {

			int case_value = GetTileMapCase(c, l);

			if (case_value != 0) {

				int x = c * BRICK_WIDTH;
				int y = l * BRICK_HEIGHT + 2 * BRICK_HEIGHT;

				SDL_Rect rect = { x, y, BRICK_WIDTH , BRICK_HEIGHT };

				SDL_Color brick_color;
				if (case_value == 1) {
					brick_color = RED;
				}
				else if (case_value == 2) {
					brick_color = BLUE_1;
				}
				else if (case_value == 3) {
					brick_color = VIOLET_2;
				}
				else if (case_value == 4) {
					brick_color = ORANGE;
				}

				SDL_SetRenderDrawColor(renderer, brick_color.r, brick_color.g, brick_color.b, brick_color.a);
				SDL_RenderFillRect(renderer, &rect);

				SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
	}

	//DRAW BAR
	int bar_x = GetBarX() - GetBarWidth() / 2;
	int bar_y = GetBarY() - GetBarHeight() / 2;

	SDL_Rect bar_rect = { bar_x, bar_y, GetBarWidth(), GetBarHeight()};

	SDL_SetRenderDrawColor(renderer, GREEN_1.r, GREEN_1.g, GREEN_1.b, GREEN_1.a);
	SDL_RenderFillRect(renderer, &bar_rect);

	SDL_SetRenderDrawColor(renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
	SDL_RenderDrawRect(renderer, &bar_rect);

	SDL_SetRenderDrawColor(renderer, ORANGE.r, ORANGE.g, ORANGE.b, ORANGE.a);

	//DRAW BALL
	int n_ball = GetNBall();
	for (int i = 0; i < n_ball; i++) {
		struct Ball ball = GetBall(i);
		SDL_RenderFillCircle(renderer, ball.x, ball.y, ball.radius);
	}

	SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
	SDL_RenderDrawLine(renderer, 0, INTERFACE_HEIGHT - 45, INTERFACE_WIDTH, INTERFACE_HEIGHT - 46);

	//Draw level text
	char lvl_value_string[3];
	sprintf_s(lvl_value_string, 3, "%d", GetLevelIndex() + 1);

	char level_string[8];
	memset(level_string, '\0', 8);

	strcat_s(level_string, 8, "Level ");
	strcat_s(level_string, 8, lvl_value_string);

	SDL_Surface* txt_level_surface = TTF_RenderText_Blended(font_roboto_25, level_string, WHITE);
	if (txt_level_surface != NULL) {
		SDL_Texture* txt_level_texture = SDL_CreateTextureFromSurface(renderer, txt_level_surface);

		SDL_Rect txt_level_position;
		txt_level_position.x = 10;
		txt_level_position.y = INTERFACE_HEIGHT - 38;
		txt_level_position.w = txt_level_surface->w;
		txt_level_position.h = txt_level_surface->h;

		SDL_RenderCopy(renderer, txt_level_texture, NULL, &txt_level_position);

		SDL_FreeSurface(txt_level_surface);
		SDL_DestroyTexture(txt_level_texture);
	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
	}

	//Draw score text
	char score_value_string[5] = { "" };
	sprintf_s(score_value_string, 5, "%d", GetScore());

	char txt_score[13] = {""};
	strcat_s(txt_score, 13, "Score : ");
	strcat_s(txt_score, 13, score_value_string);


	SDL_Surface* txt_score_surface = TTF_RenderText_Blended(font_roboto_25, txt_score, WHITE);
	if (txt_score_surface != NULL) {

		SDL_Texture* txt_score_texture = SDL_CreateTextureFromSurface(renderer, txt_score_surface);

		SDL_Rect txt_score_position;
		txt_score_position.x = 715;
		txt_score_position.y = INTERFACE_HEIGHT - 38;
		txt_score_position.w = txt_score_surface->w;
		txt_score_position.h = txt_score_surface->h;

		SDL_RenderCopy(renderer, txt_score_texture, NULL, &txt_score_position);

		SDL_FreeSurface(txt_score_surface);
		SDL_DestroyTexture(txt_score_texture);
	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", SDL_GetError());
	}

	//Draw life text
	SDL_Surface* txt_life_surface = TTF_RenderText_Blended(font_roboto_25, "Vies restantes : ", WHITE);
	if (txt_life_surface != NULL) {
		SDL_Texture* txt_life_texture = SDL_CreateTextureFromSurface(renderer, txt_life_surface);

		SDL_Rect txt_life_position;
		txt_life_position.x = 210;
		txt_life_position.y = INTERFACE_HEIGHT - 38;
		txt_life_position.w = txt_life_surface->w;
		txt_life_position.h = txt_life_surface->h;

		SDL_RenderCopy(renderer, txt_life_texture, NULL, &txt_life_position);

		SDL_FreeSurface(txt_life_surface);
		SDL_DestroyTexture(txt_life_texture);

		int hearth_x_min = 210 + txt_life_position.w;

		int n_life = GetNLife();
		for (int i = 0; i < n_life; i++) {

			SDL_Rect hearth_image_position;
			hearth_image_position.x = hearth_x_min + i * 25 + i * 12;
			hearth_image_position.y = INTERFACE_HEIGHT - 65;
			hearth_image_position.w = 32;
			hearth_image_position.h = 87;

			SDL_RenderCopy(renderer, image_hearth_texture, NULL, &hearth_image_position);
		}
	}

	//Draw timer text
	char* time_elapsed_string = GetFormattedTimeElapsed();
	SDL_Surface* txt_timer_surface = TTF_RenderText_Blended(font_roboto_18, GetFormattedTimeElapsed(), WHITE);

	if (txt_timer_surface != NULL) {

		SDL_Texture* txt_timer_texture = SDL_CreateTextureFromSurface(renderer, txt_timer_surface);

		SDL_Rect txt_timer_position;
		txt_timer_position.x = 6;
		txt_timer_position.y = 4;
		txt_timer_position.w = txt_timer_surface->w;
		txt_timer_position.h = txt_timer_surface->h;

		SDL_RenderCopy(renderer, txt_timer_texture, NULL, &txt_timer_position);

		SDL_FreeSurface(txt_timer_surface);
		SDL_DestroyTexture(txt_timer_texture);
	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", SDL_GetError());
	}

	//Draw bonus text
	SDL_Surface* txt_bonus_surface = TTF_RenderText_Blended(font_roboto_18, "Bonus actuel : ", WHITE);
	if (txt_bonus_surface != NULL) {

		SDL_Texture* txt_bonus_texture = SDL_CreateTextureFromSurface(renderer, txt_bonus_surface);

		SDL_Rect txt_bonus_position;
		txt_bonus_position.x = 450;
		txt_bonus_position.y = 4;
		txt_bonus_position.w = txt_bonus_surface->w;
		txt_bonus_position.h = txt_bonus_surface->h;

		SDL_RenderCopy(renderer, txt_bonus_texture, NULL, &txt_bonus_position);

		SDL_FreeSurface(txt_bonus_surface);
		SDL_DestroyTexture(txt_bonus_texture);
	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
	}

	enum Bonus bonus = GetBonus();
	char* bonus_name = "";

	if (bonus != NONE_BONUS) {
		if (bonus == SPEED3) {
			bonus_name = "Speed 3";
		}
	}
	else {
		bonus_name = "Aucun";
	}

	SDL_Surface* txt_bonus_val_surface = TTF_RenderText_Blended(font_roboto_18, bonus_name, WHITE);
	if (txt_bonus_val_surface != NULL) {

		SDL_Texture* txt_bonus_val_texture = SDL_CreateTextureFromSurface(renderer, txt_bonus_val_surface);

		SDL_Rect txt_bonus_val_position;
		txt_bonus_val_position.x = 575;
		txt_bonus_val_position.y = 4;
		txt_bonus_val_position.w = txt_bonus_val_surface->w;
		txt_bonus_val_position.h = txt_bonus_val_surface->h;

		SDL_RenderCopy(renderer, txt_bonus_val_texture, NULL, &txt_bonus_val_position);

		SDL_FreeSurface(txt_bonus_val_surface);
		SDL_DestroyTexture(txt_bonus_val_texture);

	}
	else {
		SDL_Log("Error TTF_RenderText_Blended : %s", TTF_GetError());
	}

	DrawMessages(renderer);
	SDL_RenderPresent(renderer);

}

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {

	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

void CreateMessages()
{
	message_start = CreateMessage();
	strcpy_s(message_start.title, 80, "Veuillez presser la touche espace pour commencer ! ");
	message_start.font_title = font_roboto_25;
	message_start.position.x = INTERFACE_WIDTH / 2;
	message_start.position.y = INTERFACE_HEIGHT / 2 - 60;

	message_speed3 = CreateMessage();
	strcpy_s(message_speed3.title, 80, "Bonus : Vitesse multiplié par 3 !");
	strcpy_s(message_speed3.subtitle, 80, "Durée : 3 secondes");
	message_speed3.font_title = font_roboto_35;
	message_speed3.font_subtitle = font_roboto_25;
	message_speed3.position.x = INTERFACE_WIDTH / 2;
	message_speed3.position.y = INTERFACE_HEIGHT / 2 - 60;
	message_speed3.space_btw_titles = 55;

	m_image_rocket.position.x = INTERFACE_WIDTH/2 - image_rocket_surface->w / 2;
	m_image_rocket.position.y = 240;
	m_image_rocket.position.w = image_rocket_surface->w;
	m_image_rocket.position.h = image_rocket_surface->h;
	m_image_rocket.texture = image_rocket_texture;

	AddImage(&message_speed3, &m_image_rocket);

	message_gameover = CreateMessage();
	strcpy_s(message_gameover.title, 80, "Game over");
	strcpy_s(message_gameover.subtitle, 80, "Presser la touche espace pour rejouer...");
	message_gameover.font_title = font_roboto_35;
	message_gameover.font_subtitle = font_roboto_25;
	message_gameover.position.x = INTERFACE_WIDTH / 2;
	message_gameover.position.y = INTERFACE_HEIGHT / 2 - 60;
	message_gameover.space_btw_titles = 55;

	message_nextlevel = CreateMessage();
	strcpy_s(message_nextlevel.title, 80, "Niveau suivant !");
	message_nextlevel.font_title = font_roboto_35;
	message_nextlevel.font_subtitle = font_roboto_25;
	message_nextlevel.position.x = INTERFACE_WIDTH / 2;
	message_nextlevel.position.y = INTERFACE_HEIGHT / 2 - 60;
	message_nextlevel.space_btw_titles = 55;

	message_newball = CreateMessage();
	strcpy_s(message_newball.title, 80, "Bonus : Nouvelle balle !");
	strcpy_s(message_newball.subtitle, 80, "Durée : infinie");
	message_newball.font_title = font_roboto_35;
	message_newball.font_subtitle = font_roboto_25;
	message_newball.position.x = INTERFACE_WIDTH / 2;
	message_newball.position.y = INTERFACE_HEIGHT / 2 - 60;
	message_newball.space_btw_titles = 55;

	message_pause = CreateMessage();
	strcpy_s(message_pause.title, 80, "Le jeu est en pause !");
	strcpy_s(message_pause.subtitle, 80, "Presser la touche p pour reprendre");
	message_pause.font_title = font_roboto_35;
	message_pause.font_subtitle = font_roboto_25;
	message_pause.position.x = INTERFACE_WIDTH / 2;
	message_pause.position.y = INTERFACE_HEIGHT / 2 - 60;
	message_pause.space_btw_titles = 55;

	m_image_pause.position.x = INTERFACE_WIDTH / 2 - image_pause_surface->w / 2;
	m_image_pause.position.y = 280;
	m_image_pause.position.w = image_pause_surface->w;
	m_image_pause.position.h = image_pause_surface->h;
	m_image_pause.texture = image_pause_texture;

	AddImage(&message_pause, &m_image_pause);
}

void ShowStartMessage()
{
	SetDisplayMessage(&message_start, true);
}

void HideStartMessage() {
	SetDisplayMessage(&message_start, false);
}

void ShowSpeed3Message() {
	SetDisplayMessage(&message_speed3, true);
}

void HideSpeed3Message() {
	SetDisplayMessage(&message_speed3, false);
}

void ShowGameoverMessage()
{
	HideAllMessages();
	SetDisplayMessage(&message_gameover, true);
}

void HideGameoverMessage()
{
	SetDisplayMessage(&message_gameover, false);
}

void ShowNextlevelMessage(int level)
{
	char level_string[3];
	memset(level_string, 3, '\0');

	sprintf_s(level_string, 3, "%d", (level + 1));

	strcpy_s(message_nextlevel.subtitle, 80, "Tu passes au niveau ");
	strcat_s(message_nextlevel.subtitle, 80, level_string);

	SetDisplayMessage(&message_nextlevel, true);
	timer_message_nextlevel = SDL_AddTimer(3000, HideNextlevelMessage, NULL);
}

void HideNextlevelMessage()
{
	SetDisplayMessage(&message_nextlevel, false);
	SDL_RemoveTimer(timer_message_nextlevel);
}

void ShowNewballMessage()
{
	SetDisplayMessage(&message_newball, true);
	timer_message_newball = SDL_AddTimer(3000, HideNewballMessage, NULL);
}

void HideNewballMessage()
{
	SetDisplayMessage(&message_newball, false);
	SDL_RemoveTimer(timer_message_newball);
}

void ShowPauseMessage()
{
	SetDisplayMessage(&message_pause, true);
}

void HidePauseMessage()
{
	SetDisplayMessage(&message_pause, false);
}



