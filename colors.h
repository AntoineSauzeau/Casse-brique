#pragma once

#include <stdbool.h>

const static SDL_Color RED = { 235, 77, 75, 1 };
const static SDL_Color GREY = { 46, 40, 42 };
const static SDL_Color GREEN_1 = { 46, 204, 113 }; //EMERALD
const static SDL_Color GREEN_2 = { 39, 174, 96, 1.0 }; //NEPHRITIS
const static SDL_Color BLACK = { 0, 0, 0 };
const static SDL_Color BLUE_1 = { 52, 152, 219 };
const static SDL_Color WHITE = { 255, 255, 255 };
const static SDL_Color VIOLET = { 44, 62, 80, 1 };
const static SDL_Color VIOLET_2 = { 155, 89, 182,1.0 }; //AMETHYST
const static SDL_Color ORANGE = { 243, 156, 18, 1 };
const static SDL_Color TRANSPARENT = { 0, 0, 0, 100 };

static bool SameColor(SDL_Color c1, SDL_Color c2)
{
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}