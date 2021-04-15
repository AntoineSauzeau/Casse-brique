#include "interface.h"

#include "SDL.h"

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "interface_game.h"
#include "interface_menu.h"
#include "text_switch_widget.h"
#include "message.h"
#include "sound.h"



static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_Thread* update_thread = NULL;
static bool stop_threads = false;

static int interface_index = 0;			//0 = GameInterface - 1 = MenuInterface



/*
	Prototypes.
*/

static void UpdateLoop();



void CreateInterface()
{
	window = SDL_CreateWindow("Casse brique", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INTERFACE_WIDTH, INTERFACE_HEIGHT, NULL);
	if (window == NULL) {
		fprintf(stderr, "Error SDL_CreateWindow : %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == NULL) {
		fprintf(stderr, "Error SDL_CreateRenderer : %s", SDL_GetError());
	}

	MSG_InitModule();
	TSW_InitModule(renderer);
	Audio_InitModule();

	Audio_PlayMusic(0, -1);

	CreateGameInterface(renderer);
	CreateMenuInterface(renderer);

}

void DestroyInterface() {

	stop_threads = true;
	SDL_WaitThread(update_thread, NULL);

	DestroyGameInterface();
	DestroyMenuInterface();

	MSG_FreeModule();
	TSW_FreeModule();
	Audio_FreeModule();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void SetInterfacePage(int index)
{
	interface_index = index;
}



void StartEventLoop() {

	SDL_Event event;

	while (!stop_threads) {

		while (SDL_PollEvent(&event)) {

			switch (event.type) {
			case SDL_QUIT:
			{
				return;
			}
			default:
			{
				if (interface_index == 0) {
					GameInterfaceEvent(&event);
				}
				else if (interface_index == 1) {
					MenuInterfaceEvent(&event);
				}
				break;
			}
			}
		}
	}
}

void StartUpdateLoop() {

	update_thread = SDL_CreateThread(UpdateLoop, "UpdateThread", NULL);
	if (update_thread == NULL) {
		fprintf(stderr, "Erreur lors du lancement du thread d'affichage");

		ExitBreakout(EXIT_FAILURE);
	}
}

void UpdateLoop()
{
	clock_t start_t, end_t, total_t;

	while (!stop_threads) {

		start_t = clock();

		if (interface_index == 0) {
			UpdateGameInterface();
		}
		else if (interface_index == 1) {
			UpdateMenuInterface();
		}

		end_t = clock();
		total_t = ((double)end_t - (double)start_t) / CLOCKS_PER_SEC;

		double time_to_wait = 1. / FPS - total_t;
		time_to_wait = time_to_wait * 1000;

		if (time_to_wait < 0) {
			continue;
		}
		else {
			SDL_Delay(time_to_wait);
		}

	}
}
