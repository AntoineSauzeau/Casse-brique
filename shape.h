#pragma once

#include "SDL_render.h"
#include "SDL_rect.h"

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
void SDL_RenderDrawThickRect(SDL_Renderer* renderer, SDL_Rect* rect, int thickness);
void SDL_RenderDrawDashedLine(SDL_Renderer* renderer, SDL_Rect rect, int dash_length, int space_length);