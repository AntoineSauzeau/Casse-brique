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

	int new_size = (tsw->n_value + 1) * 50 * sizeof(char);
	char** ptr_tmp = NULL;
	ptr_tmp = realloc(tsw->l_value, new_size);

	if (ptr_tmp == NULL) {

		//ShowError("Erreur realloc : %s", errno);
		printf("qsdsq");

		free(ptr_tmp);
		ExitBreakout(EXIT_FAILURE);
	}

	tsw->l_value = ptr_tmp;
	printf("aa");

	tsw->n_value++;
	if (tsw->l_value != NULL) {
		memset(tsw->l_value[tsw->n_value - 1], '\0', 50);
		strcpy_s(tsw->l_value[tsw->n_value - 1], 15, "value");
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

void TSW_Next(TSW* tsw) {

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

void TSW_Draw(TSW* tsw) {

	SDL_Surface* text_value_surface = TTF_RenderText_Blended(tsw->font, tsw->l_value[tsw->index], tsw->color);
	if (text_value_surface != NULL) {

		SDL_Texture* text_value_texture = SDL_CreateTextureFromSurface(renderer, text_value_surface);

		struct SDL_Rect text_value_pos;
		text_value_pos.x = tsw->pos.x - text_value_surface->w / 2;
		text_value_pos.y = tsw->pos.y - text_value_surface->h / 2;
		text_value_pos.w = text_value_surface->w;
		text_value_pos.h = text_value_surface->h;

		SDL_RenderCopy(renderer, text_value_texture, NULL, &text_value_pos);

		SDL_DestroyTexture(text_value_texture);
	}
	else {
		fprintf(stderr, "Error TTF_RenderText_Blended : %s", TTF_GetError());
	}

	SDL_Rect left_arrow_pos;
	left_arrow_pos.x = tsw->pos.x - text_value_surface->w / 2 - tsw->padding - left_arrow_surface->w;
	left_arrow_pos.y = tsw->pos.y - text_value_surface->h / 2;
	left_arrow_pos.w = left_arrow_surface->w;
	left_arrow_pos.h = left_arrow_surface->h;

	SDL_RenderCopy(renderer, left_arrow_texture, NULL, &left_arrow_pos);

	SDL_Rect right_arrow_pos;
	right_arrow_pos.x = tsw->pos.x + text_value_surface->w / 2 + tsw->padding;
	right_arrow_pos.y = tsw->pos.y - text_value_surface->h / 2;
	right_arrow_pos.w = right_arrow_surface->w;
	right_arrow_pos.h = right_arrow_surface->h;

	SDL_RenderCopy(renderer, right_arrow_texture, NULL, &right_arrow_pos);

	SDL_FreeSurface(text_value_surface);
}
