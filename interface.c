#include "interface.h"

#include "SDL.h"
#include "interface_game.h"
#include "game.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void CreateInterface()
{
	window = SDL_CreateWindow("Casse brique", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INTERFACE_WIDTH, INTERFACE_HEIGHT, NULL);
	if (window == NULL) {
		printf("Error SDL_CreateWindow : %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) {
		printf("Error SDL_CreateRenderer : %s", SDL_GetError());
	}

	CreateGameInterface(renderer);
	StartGameLoop();
}

void DestroyInterface() {

	DestroyGameInterface();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}



void StartEventLoop() {

	SDL_Event event;

	while (1) {

		while (SDL_PollEvent(&event)) {

			switch (event.type) {
			case SDL_QUIT:
			{
				return;
			}
			default:
			{
				GameEvent(event);
			}
			}
		}
	}
}