#include "interface_menu.h"

#include "interface.h"
#include "colors.h"
#include "text_switch_widget.h"
#include "shape.h"

#include <SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

static SDL_Renderer* renderer = NULL;

static TSW tsw_page;
static TTF_Font* font_roboto_23 = NULL;

static SDL_Surface* img_background_surface = NULL;
static SDL_Texture* img_background_texture = NULL;

static SDL_Surface* img_keyP_surface = NULL;
static SDL_Surface* img_keyR_surface = NULL;
static SDL_Surface* img_keyPlus_surface = NULL;
static SDL_Surface* img_keyMinus_surface = NULL;
static SDL_Surface* img_keyEchap_surface = NULL;
static SDL_Surface* img_keyN_surface = NULL;

static SDL_Texture* img_keyP_texture = NULL;
static SDL_Texture* img_keyR_texture = NULL;
static SDL_Texture* img_keyPlus_texture = NULL;
static SDL_Texture* img_keyMinus_texture = NULL;
static SDL_Texture* img_keyEchap_texture = NULL;
static SDL_Texture* img_keyN_texture = NULL;

/*
*	Prototypes. 
*/

static void DrawMenuInterface();
static void CreateTSWs();

static void LoadFonts();
static void FreeFonts();
static void LoadImages();
static void FreeImages();

static void DrawKeyPage();


void CreateMenuInterface(SDL_Renderer* _renderer)
{
	renderer = _renderer;

	LoadFonts();
	LoadImages();
	CreateTSWs();

}

void DestroyMenuInterface()
{
	FreeFonts();
	FreeImages();
}

void LoadFonts()
{
	font_roboto_23 = TTF_OpenFont("Roboto-Medium.ttf", 23);
	if (!font_roboto_23) {
		fprintf(stderr, "Error TTF_OpenFont : %s", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}
}

void FreeFonts() 
{
	TTF_CloseFont(font_roboto_23);
}

void LoadImages()
{
	img_background_surface = IMG_Load("Images/menu_background.jpg");
	if (!img_background_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_background_texture = SDL_CreateTextureFromSurface(renderer, img_background_surface);

	img_keyP_surface = IMG_Load("Images/Keys/key_P.png");
	if (!img_keyP_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyP_texture = SDL_CreateTextureFromSurface(renderer, img_keyP_surface);

	img_keyR_surface = IMG_Load("Images/Keys/key_R.png");
	if (!img_keyR_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyR_texture = SDL_CreateTextureFromSurface(renderer, img_keyR_surface);

	img_keyPlus_surface = IMG_Load("Images/Keys/key_plus.png");
	if (!img_keyPlus_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyPlus_texture = SDL_CreateTextureFromSurface(renderer, img_keyPlus_surface);

	img_keyMinus_surface = IMG_Load("Images/Keys/key_minus.png");
	if (!img_keyMinus_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyMinus_texture = SDL_CreateTextureFromSurface(renderer, img_keyMinus_surface);

	img_keyEchap_surface = IMG_Load("Images/Keys/key_echap.png");
	if (!img_keyEchap_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyEchap_texture = SDL_CreateTextureFromSurface(renderer, img_keyEchap_surface);

	img_keyN_surface = IMG_Load("Images/Keys/key_N.png");
	if (!img_keyN_surface) {
		fprintf(stderr, "Errror IMG_Load : %s", IMG_GetError());
	}

	img_keyN_texture = SDL_CreateTextureFromSurface(renderer, img_keyN_surface);
}

void FreeImages() 
{
	SDL_FreeSurface(img_background_surface);
	SDL_FreeSurface(img_keyP_surface);
	SDL_FreeSurface(img_keyR_surface);
	SDL_FreeSurface(img_keyPlus_surface);
	SDL_FreeSurface(img_keyMinus_surface);
	SDL_FreeSurface(img_keyEchap_surface);
	SDL_FreeSurface(img_keyN_surface);

	SDL_DestroyTexture(img_background_texture);
	SDL_DestroyTexture(img_keyP_texture);
	SDL_DestroyTexture(img_keyR_texture);
	SDL_DestroyTexture(img_keyPlus_texture);
	SDL_DestroyTexture(img_keyMinus_texture);
	SDL_DestroyTexture(img_keyEchap_texture);
	SDL_DestroyTexture(img_keyN_texture);
}

void MenuInterfaceEvent(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONUP) {
		TSW_UpdateAll();
	}
}

void UpdateMenuInterface()
{
	DrawMenuInterface();
}

void DrawMenuInterface() 
{

	SDL_RenderClear(renderer);

	SDL_Rect img_background_pos;
	img_background_pos.x = 0;
	img_background_pos.y = 0;
	img_background_pos.w = img_background_surface->w;
	img_background_pos.h = img_background_surface->h;

	SDL_RenderCopy(renderer, img_background_texture, NULL, &img_background_pos);

	TSW_DrawAll();

	if (strcmp(TSW_GetValue(&tsw_page), "Touches") == 0) {
		DrawKeyPage();
	}

	SDL_RenderPresent(renderer);
}

void DrawKeyPage() {

	int img_key_width = img_keyP_surface->w / 2.2;
	int img_key_height = img_keyP_surface->h / 2.2;


	SDL_Rect img_keyP_pos;
	img_keyP_pos.x = 85 - img_key_width / 2;
	img_keyP_pos.y = 150 - img_key_height / 2;
	img_keyP_pos.w = img_key_width;
	img_keyP_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyP_texture, NULL, &img_keyP_pos);

	SDL_Surface* txt_break_surface = TTF_RenderText_Blended(font_roboto_23, "Met le jeu en pause", WHITE);
	if (txt_break_surface != NULL) {

		SDL_Texture* txt_break_texture = SDL_CreateTextureFromSurface(renderer, txt_break_surface);

		SDL_Rect txt_break_pos;
		txt_break_pos.x = 265 - txt_break_surface->w / 2;
		txt_break_pos.y = 150 - txt_break_surface->h / 2;
		txt_break_pos.w = txt_break_surface->w;
		txt_break_pos.h = txt_break_surface->h;

		SDL_RenderCopy(renderer, txt_break_texture, NULL, &txt_break_pos);

		SDL_FreeSurface(txt_break_surface);
		SDL_DestroyTexture(txt_break_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}



	SDL_Rect img_keyR_pos;
	img_keyR_pos.x = 85 - img_key_width / 2;
	img_keyR_pos.y = 225 - img_key_height / 2;
	img_keyR_pos.w = img_key_width;
	img_keyR_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyR_texture, NULL, &img_keyR_pos);

	SDL_Surface* txt_restart_surface = TTF_RenderText_Blended(font_roboto_23, "Recommence la partie", WHITE);
	if (txt_restart_surface != NULL) {

		SDL_Texture* txt_restart_texture = SDL_CreateTextureFromSurface(renderer, txt_restart_surface);

		SDL_Rect txt_restart_pos;
		txt_restart_pos.x = 265 - txt_restart_surface->w / 2;
		txt_restart_pos.y = 225 - txt_restart_surface->h / 2;
		txt_restart_pos.w = txt_restart_surface->w;
		txt_restart_pos.h = txt_restart_surface->h;

		SDL_RenderCopy(renderer, txt_restart_texture, NULL, &txt_restart_pos);

		SDL_FreeSurface(txt_restart_surface);
		SDL_DestroyTexture(txt_restart_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}



	SDL_Rect img_keyPlus_pos;
	img_keyPlus_pos.x = 560 - img_key_width / 2;
	img_keyPlus_pos.y = 150 - img_key_height / 2;
	img_keyPlus_pos.w = img_key_width;
	img_keyPlus_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyPlus_texture, NULL, &img_keyPlus_pos);

	SDL_Surface* txt_plus_surface = TTF_RenderText_Blended(font_roboto_23, "Augmente le son", WHITE);
	if (txt_plus_surface != NULL) {

		SDL_Texture* txt_plus_texture = SDL_CreateTextureFromSurface(renderer, txt_plus_surface);

		SDL_Rect txt_plus_pos;
		txt_plus_pos.x = 740 - txt_plus_surface->w / 2;
		txt_plus_pos.y = 150 - txt_plus_surface->h / 2;
		txt_plus_pos.w = txt_plus_surface->w;
		txt_plus_pos.h = txt_plus_surface->h;

		SDL_RenderCopy(renderer, txt_plus_texture, NULL, &txt_plus_pos);

		SDL_FreeSurface(txt_plus_surface);
		SDL_DestroyTexture(txt_plus_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}



	SDL_Rect img_keyMinus_pos;
	img_keyMinus_pos.x = 560 - img_key_width / 2;
	img_keyMinus_pos.y = 225 - img_key_height / 2;
	img_keyMinus_pos.w = img_key_width;
	img_keyMinus_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyMinus_texture, NULL, &img_keyMinus_pos);

	SDL_Surface* txt_minus_surface = TTF_RenderText_Blended(font_roboto_23, "Diminue le son", WHITE);
	if (txt_minus_surface != NULL) {

		SDL_Texture* txt_minus_texture = SDL_CreateTextureFromSurface(renderer, txt_minus_surface);

		SDL_Rect txt_minus_pos;
		txt_minus_pos.x = 740 - txt_minus_surface->w / 2;
		txt_minus_pos.y = 225 - txt_minus_surface->h / 2;
		txt_minus_pos.w = txt_minus_surface->w;
		txt_minus_pos.h = txt_minus_surface->h;

		SDL_RenderCopy(renderer, txt_minus_texture, NULL, &txt_minus_pos);

		SDL_FreeSurface(txt_minus_surface);
		SDL_DestroyTexture(txt_minus_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}



	SDL_Rect img_keyEchap_pos;
	img_keyEchap_pos.x = 290 - img_key_width / 2;
	img_keyEchap_pos.y = 315 - img_key_height / 2;
	img_keyEchap_pos.w = img_key_width;
	img_keyEchap_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyEchap_texture, NULL, &img_keyEchap_pos);

	SDL_Surface* txt_echap_surface = TTF_RenderText_Blended(font_roboto_23, "Ouvre le menu / Reviens en jeu", WHITE);
	if (txt_echap_surface != NULL) {

		SDL_Texture* txt_echap_texture = SDL_CreateTextureFromSurface(renderer, txt_echap_surface);

		SDL_Rect txt_echap_pos;
		txt_echap_pos.x = 490 - txt_echap_surface->w / 2;
		txt_echap_pos.y = 315 - txt_echap_surface->h / 2;
		txt_echap_pos.w = txt_echap_surface->w;
		txt_echap_pos.h = txt_echap_surface->h;

		SDL_RenderCopy(renderer, txt_echap_texture, NULL, &txt_echap_pos);

		SDL_FreeSurface(txt_echap_surface);
		SDL_DestroyTexture(txt_echap_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}



	SDL_Surface* txt_cheat_surface = TTF_RenderText_Blended(font_roboto_23, "Options de triche", WHITE);
	if (txt_cheat_surface != NULL) {

		SDL_Texture* txt_cheat_texture = SDL_CreateTextureFromSurface(renderer, txt_cheat_surface);

		SDL_Rect txt_cheat_pos;
		txt_cheat_pos.x = INTERFACE_WIDTH / 2 - txt_cheat_surface->w / 2;
		txt_cheat_pos.y = 455 - txt_cheat_surface->h / 2;
		txt_cheat_pos.w = txt_cheat_surface->w;
		txt_cheat_pos.h = txt_cheat_surface->h;

		SDL_RenderCopy(renderer, txt_cheat_texture, NULL, &txt_cheat_pos);

		SDL_FreeSurface(txt_cheat_surface);
		SDL_DestroyTexture(txt_cheat_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}

	SDL_Rect cheat_frame_rect;
	cheat_frame_rect.x = INTERFACE_WIDTH / 2 - 200;
	cheat_frame_rect.y = 430;
	cheat_frame_rect.w = 400;
	cheat_frame_rect.h = 50;
	
	SDL_RenderDrawRect(renderer, &cheat_frame_rect);



	SDL_Rect img_keyN_pos;
	img_keyN_pos.x = 290 - img_key_width / 2;
	img_keyN_pos.y = 520 - img_key_height / 2;
	img_keyN_pos.w = img_key_width;
	img_keyN_pos.h = img_key_height;

	SDL_RenderCopy(renderer, img_keyN_texture, NULL, &img_keyN_pos);

	SDL_Surface* txt_next_surface = TTF_RenderText_Blended(font_roboto_23, "Téléporte au niveau suivant", WHITE);
	if (txt_next_surface != NULL) {

		SDL_Texture* txt_next_texture = SDL_CreateTextureFromSurface(renderer, txt_next_surface);

		SDL_Rect txt_next_pos;
		txt_next_pos.x = 490 - txt_next_surface->w / 2;
		txt_next_pos.y = 520 - txt_next_surface->h / 2;
		txt_next_pos.w = txt_next_surface->w;
		txt_next_pos.h = txt_next_surface->h;

		SDL_RenderCopy(renderer, txt_next_texture, NULL, &txt_next_pos);

		SDL_FreeSurface(txt_next_surface);
		SDL_DestroyTexture(txt_next_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}


}

void CreateTSWs() 
{

	tsw_page = TSW_Create();
	TSW_AddValue(&tsw_page, "Touches");
	TSW_AddValue(&tsw_page, "Reglages");
	TSW_AddValue(&tsw_page, "Classement");
	TSW_AddValue(&tsw_page, "Sauvegardes");
	TSW_AddValue(&tsw_page, "Statistiques");

	
	tsw_page.pos.x = INTERFACE_WIDTH / 2;
	tsw_page.pos.y = 60;
	tsw_page.font = font_roboto_23;
	tsw_page.arrow_size_ratio = 0.3;

	TSW_Show(&tsw_page);
}

