#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

SDL_Color WHITE = { 255, 255, 255, 0 };
SDL_Color BLACK = { 0, 0, 0, 0 };
SDL_Color TRANSPARENT = { 0, 0, 0, 100 };

struct Message {

	char* title;
	char* subtitle;
	SDL_Color title_color;
	SDL_Color subtitle_color;

	int title_size;
	int subtitle_size;

	TTF_Font* font;

	SDL_Color background_color;
	SDL_Color border_color;

	int border_thickness;

	int padding;
	int space_btw_titles;

	SDL_Point position;

};

void DrawMessage(SDL_Renderer* renderer, struct Message* message);
int MessageWidth(struct Message* message);
int MessageHeight(struct Message* message);
struct Message CreateMessage();

