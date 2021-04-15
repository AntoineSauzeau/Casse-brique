#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <stdlib.h> 
#include <stdbool.h>

typedef struct {
	SDL_Texture* texture;
	SDL_Rect position;
} MessageImage;

typedef struct {

	char title[80];
	char subtitle[80];
	SDL_Color title_color;
	SDL_Color subtitle_color;

	TTF_Font* font_title;
	TTF_Font* font_subtitle;

	SDL_Color background_color;
	SDL_Color border_color;

	int border_thickness;

	int padding;
	int space_btw_titles;

	SDL_Point position;

	MessageImage** l_message_image;
	SDL_mutex* l_message_image_mutex;
	int n_image;
} Message;

void MSG_InitModule();
void MSG_FreeModule();

void MSG_Draw(SDL_Renderer* renderer, Message* message);
void MSG_DrawAll(SDL_Renderer* renderer);

Message MSG_Create();
bool MSG_Same(Message* m1, Message* m2);

bool MSG_Displayed(Message* message);
void MSG_HideAll();

void MSG_Show(Message* message);
void MSG_Hide(Message* message);

void MSG_AddImage(Message* message, MessageImage* m_image);
void MSG_RemoveImage(Message* message, MessageImage* m_image);

