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

void InitMessages();
void DestroyMessages();

void DrawMessage(SDL_Renderer* renderer, Message* message);
void DrawMessages(SDL_Renderer* renderer);

int MessageWidth(SDL_Renderer* renderer, Message* message);
int MessageHeight(SDL_Renderer* renderer, Message* message);

Message CreateMessage();
bool SameMessage(Message* m1, Message* m2);

void SetDisplayMessage(Message* message, bool displayed);
bool MessageDisplayed(Message* message);
void HideAllMessages();

void DrawImages(SDL_Renderer* renderer, Message* message);
void AddImage(Message* message, MessageImage* m_image);
void RemoveImage(Message* message, MessageImage* m_image);

