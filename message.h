#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

#include "colors.h"

struct Message {

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

	struct MessageImage** l_message_image;
	SDL_mutex* l_message_image_mutex;
	int n_image;
};

struct MessageImage{
	SDL_Texture* texture;
	SDL_Rect position;
};

void InitMessages();
void DestroyMessages();

void DrawMessage(SDL_Renderer* renderer, struct Message* message);
void DrawMessages(SDL_Renderer* renderer);

int MessageWidth(SDL_Renderer* renderer, struct Message* message);
int MessageHeight(SDL_Renderer* renderer, struct Message* message);

struct Message CreateMessage();
void SetDisplayMessage(struct Message* message, bool displayed);
bool MessageDisplayed(struct Message* message);
bool SameMessage(struct Message* m1, struct Message* m2);

void DrawImages(SDL_Renderer* renderer, struct Message* message);
void AddImage(struct Message* message, struct MessageImage* m_image);
void RemoveImage(struct Message* message, struct MessageImage* m_image);

