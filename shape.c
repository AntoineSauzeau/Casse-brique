#include "shape.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) 
{

	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

void SDL_RenderDrawThickRect(SDL_Renderer* renderer, SDL_Rect* rect, int thickness) 
{
	for (int i = 0; i < thickness; i++) {

		rect->x = rect->x - i;
		rect->y = rect->y - i;
		rect->w = rect->w + i * 2;
		rect->h = rect->h + i * 2;

		SDL_RenderDrawRect(renderer, rect);
	}
}

void SDL_RenderDrawDashedLine(SDL_Renderer* renderer, SDL_Rect rect, int dash_length, int space_length) {

	int direction;			//0: horizontal, 1: vertical
	if (rect.w < rect.h) {
		direction = 1;
	}
	else {
		direction = 0;
	}

	int n;
	if (direction == 0) {
		n = floor(rect.w / (dash_length + space_length));
	}
	else {
		n = floor(rect.h / (dash_length + space_length));
	}

	for (int i = 0; i < n; i++) {

		int x1, x2, y1, y2;

		if (direction == 0) {
			x1 = rect.x + i * (dash_length + space_length);
			x2 = x1 + dash_length;
			y1 = rect.y;
			y2 = y1 + (rect.h - 1);
		}
		else {
			x1 = rect.x;
			x2 = x1 + (rect.w - 1);
			y1 = rect.x + i * (dash_length + space_length);
			y2 = y1 + dash_length;
		}

		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}
}