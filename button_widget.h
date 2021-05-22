#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

typedef struct {
	char text[50];
	TTF_Font* font;
	int padding;
	int border_thickness;
	SDL_Color text_color;
	SDL_Point pos;
} Button;


Button BTN_Create();

void BTN_Show(Button* button);
void BTN_Hide(Button* button);
void BTN_HideAll();

void BTN_Draw(Button* button, SDL_Renderer* renderer);
void BTN_DrawAll(SDL_Renderer* renderer);
