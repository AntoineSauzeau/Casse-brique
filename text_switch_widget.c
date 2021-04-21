#include "text_switch_widget.h"
#include "colors.h"

#include "SDL/SDL_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>


static TSW** l_tsw = NULL;
static int n_tsw = 0;

static SDL_Renderer* renderer = NULL;

static SDL_Surface* left_arrow_surface = NULL;
static SDL_Surface* right_arrow_surface = NULL;
static SDL_Texture* left_arrow_texture = NULL;
static SDL_Texture* right_arrow_texture = NULL;

/*
*	Prototypes.
*/
static void TSW_Draw(TSW* tsw);
static bool TSW_InLeftArrowBounds(TSW* tsw, int x, int y);
static bool TSW_InRightArrowBounds(TSW* tsw, int x, int y);




void TSW_InitModule(SDL_Renderer* _renderer)
{
	renderer = _renderer;

	left_arrow_surface = IMG_Load("left-arrow.png");
	if (left_arrow_surface == NULL) {
		fprintf(stderr, "Error IMG_Load : %s", IMG_GetError());
	}

	left_arrow_texture = SDL_CreateTextureFromSurface(renderer, left_arrow_surface);

	right_arrow_surface = IMG_Load("right-arrow.png");
	if (right_arrow_surface == NULL) {
		fprintf(stderr, "Error IMG_Load : %s", IMG_GetError());
	}

	right_arrow_texture = SDL_CreateTextureFromSurface(renderer, right_arrow_surface);
}

void TSW_FreeModule()
{
	SDL_FreeSurface(left_arrow_surface);
	SDL_FreeSurface(right_arrow_surface);
	SDL_DestroyTexture(left_arrow_surface);
	SDL_DestroyTexture(right_arrow_surface);

	free(l_tsw);
}

TSW TSW_Create()
{
	TSW tsw;
	tsw.l_value = NULL;
	tsw.n_value = 0;
	tsw.index = 0;

	tsw.font = NULL;
	tsw.arrow_size_ratio = 1.;
	tsw.padding = 10;
	tsw.color = WHITE;

	return tsw;
}

void TSW_AddValue(TSW* tsw, char* value)
{
	errno = 0;

	char** ptr_tmp_1 = NULL;
	char* ptr_tmp_2 = NULL;

	ptr_tmp_1 = (char **) realloc(tsw->l_value, (tsw->n_value + 1) * ((int) sizeof(char *)));
	if (ptr_tmp_1 != NULL) {

		tsw->l_value = ptr_tmp_1;

		for (int i = 0; i < (tsw->n_value + 1); i++) {

			if (i == tsw->n_value) {
				tsw->l_value[i] = (char *) malloc(50 * sizeof(char));
			}
			else {
				ptr_tmp_2 = (char*) realloc(tsw->l_value[i], 50 * (int) sizeof(char));
				if (ptr_tmp_2 != NULL) {
					tsw->l_value[i] = ptr_tmp_2;
				}
				else {
					break;
				}
			}
		}
	}

	if (errno != 0) {

		ShowError("Erreur realloc : %s", errno);

		for (int i = 0; i < tsw->n_value; i++) {
			free(tsw->l_value[i]);
		}

		free(ptr_tmp_1);
		free(ptr_tmp_2);

		ExitBreakout(EXIT_FAILURE);
	}

	tsw->n_value++;
	if (tsw->l_value != NULL) {
		memset(tsw->l_value[(tsw->n_value - 1)], '\0', 50);
		strcpy_s(tsw->l_value[(tsw->n_value - 1)], 50, value);
	}
}

void TSW_RemoveValue(TSW* tsw, char* value) 
{
	errno = 0;

	bool shift = false;
	for (int i = 0; i < (tsw->n_value - 1); i++){

		if (strcmp(tsw->l_value[i], value) == 0) {
			shift = true;
		}
		
		if (shift) {
			strcpy_s(tsw->l_value[i], 50, tsw->l_value[i + 1]);
		}
	}

	int new_size = (tsw->n_value - 1) * sizeof(char[50]);
	TSW** ptr_tmp = NULL;
	ptr_tmp = realloc(tsw->l_value, new_size);

	if (ptr_tmp == NULL) {

		ShowError("Erreur realloc : %s", errno);

		free(ptr_tmp);
		ExitBreakout(EXIT_FAILURE);
	}

	tsw->l_value = ptr_tmp;

	tsw->n_value--;
}

void TSW_Next(TSW* tsw) 
{

	if (tsw->index + 1 == tsw->n_value) {
		tsw->index = 0;
	}
	else {
		tsw->index++;
	}
}

void TSW_Previous(TSW* tsw) 
{
	if (tsw->index - 1 == -1) {
		tsw->index = tsw->n_value - 1;
	}
	else {
		tsw->index--;
	}
}

void TSW_UpdateAll() 
{
	
	int x, y = 0;
	SDL_GetMouseState(&x, &y);

	for (int i = 0; i < n_tsw; i++) {
		
		if (TSW_InLeftArrowBounds(l_tsw[i], x, y)) {
			TSW_Previous(l_tsw[i]);
		}
		else if (TSW_InRightArrowBounds(l_tsw[i], x, y)) {
			TSW_Next(l_tsw[i]);
		}
	}
}

bool TSW_InLeftArrowBounds(TSW* tsw, int x, int y)
{
	if (tsw->n_value == 0) {
		return false;
	}

	int arrow_width = left_arrow_surface->w * tsw->arrow_size_ratio;
	int arrow_height = left_arrow_surface->h * tsw->arrow_size_ratio;

	SDL_Surface* text_value_surface = TTF_RenderText_Blended(tsw->font, tsw->l_value[tsw->index], tsw->color);
	if (text_value_surface == NULL) {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
		return;
	}

	SDL_Rect left_arrow_pos;
	left_arrow_pos.x = tsw->pos.x - text_value_surface->w / 2 - tsw->padding - arrow_width;
	left_arrow_pos.y = tsw->pos.y - arrow_height / 2;
	left_arrow_pos.w = arrow_width;
	left_arrow_pos.h = arrow_height;

	SDL_Rect mouse_pos;
	mouse_pos.x = x;
	mouse_pos.y = y;
	mouse_pos.w = 1;
	mouse_pos.h = 1;

	return SDL_HasIntersection(&left_arrow_pos, &mouse_pos);

}

bool TSW_InRightArrowBounds(TSW* tsw, int x, int y) 
{
	if (tsw->n_value == 0) {
		return false;
	}

	int arrow_width = left_arrow_surface->w * tsw->arrow_size_ratio;
	int arrow_height = left_arrow_surface->h * tsw->arrow_size_ratio;

	SDL_Surface* text_value_surface = TTF_RenderText_Blended(tsw->font, tsw->l_value[tsw->index], tsw->color);
	if (text_value_surface == NULL) {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
		return;
	}

	SDL_Rect right_arrow_pos;
	right_arrow_pos.x = tsw->pos.x + text_value_surface->w / 2 + tsw->padding;
	right_arrow_pos.y = tsw->pos.y - arrow_height / 2;
	right_arrow_pos.w = arrow_width;
	right_arrow_pos.h = arrow_height;

	SDL_Rect mouse_pos;
	mouse_pos.x = x;
	mouse_pos.y = y;
	mouse_pos.w = 1;
	mouse_pos.h = 1;

	return SDL_HasIntersection(&right_arrow_pos, &mouse_pos);
}

void TSW_Show(TSW* tsw)
{
	errno = 0;

	int new_size = (n_tsw + 1) * sizeof(TSW*);
	TSW** ptr_tmp = NULL;
	ptr_tmp = realloc(l_tsw, new_size);

	if (ptr_tmp == NULL) {

		ShowError("Erreur realloc : %s", errno);

		free(ptr_tmp);
		ExitBreakout(EXIT_FAILURE);
	}

	l_tsw = ptr_tmp;

	n_tsw++;
	if (l_tsw != NULL) {
		l_tsw[n_tsw - 1] = tsw;
	}
}

void TSW_Hide(TSW* tsw)
{
	errno = 0;

	bool shift = false;
	for (int i = 0; i < (n_tsw - 1); i++) {

		if (l_tsw[i] == tsw) {
			shift = true;
		}

		if (shift) {
			l_tsw[i] = l_tsw[i + 1];
		}
	}

	int new_size = (n_tsw + 1) * sizeof(TSW*);
	TSW** ptr_tmp = NULL;
	ptr_tmp = realloc(l_tsw, new_size);

	if (ptr_tmp == NULL) {

		ShowError("Erreur realloc : %s", errno);

		free(ptr_tmp);
		ExitBreakout(EXIT_FAILURE);
	}

	l_tsw = ptr_tmp;

	n_tsw--;
	if (l_tsw != NULL) {
		l_tsw[n_tsw - 1] = tsw;
	}
}

void TSW_Draw(TSW* tsw) 
{

	if (tsw->n_value == 0) {
		return;
	}

	int arrow_width = left_arrow_surface->w * tsw->arrow_size_ratio;
	int arrow_height = left_arrow_surface->h * tsw->arrow_size_ratio;

	SDL_Surface* text_value_surface = TTF_RenderText_Blended(tsw->font, tsw->l_value[tsw->index], tsw->color);
	if (text_value_surface == NULL) {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
		return;
	}

	SDL_Texture* text_value_texture = SDL_CreateTextureFromSurface(renderer, text_value_surface);

	struct SDL_Rect text_value_pos;
	text_value_pos.x = tsw->pos.x - text_value_surface->w / 2;
	text_value_pos.y = tsw->pos.y - text_value_surface->h / 2;
	text_value_pos.w = text_value_surface->w;
	text_value_pos.h = text_value_surface->h;

	SDL_RenderCopy(renderer, text_value_texture, NULL, &text_value_pos);

	SDL_DestroyTexture(text_value_texture);

	SDL_Rect left_arrow_pos;
	left_arrow_pos.x = tsw->pos.x - text_value_surface->w / 2 - tsw->padding - arrow_width;
	left_arrow_pos.y = tsw->pos.y - arrow_height / 2;
	left_arrow_pos.w = arrow_width;
	left_arrow_pos.h = arrow_height;

	SDL_RenderCopy(renderer, left_arrow_texture, NULL, &left_arrow_pos);

	SDL_Rect right_arrow_pos;
	right_arrow_pos.x = tsw->pos.x + text_value_surface->w / 2 + tsw->padding;
	right_arrow_pos.y = tsw->pos.y - arrow_height / 2;
	right_arrow_pos.w = arrow_width;
	right_arrow_pos.h = arrow_height;

	SDL_RenderCopy(renderer, right_arrow_texture, NULL, &right_arrow_pos);

	SDL_FreeSurface(text_value_surface);
}

void TSW_DrawAll() 
{
	for (int i = 0; i < n_tsw; i++) {
		TSW_Draw(l_tsw[i]);
	}
}

char* TSW_GetValue(TSW* tsw)
{
	return tsw->l_value[tsw->index];
}