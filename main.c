#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"


void FreeBreakout();
extern void ExitBreakout(int);

void FreeBreakout() {
	DestroyInterface();
	SDL_Quit();
	TTF_Quit();
}

void ExitBreakout(int code) {
	FreeBreakout();
	exit(code);
}

int main(int argv, char** args) {

	printf("Starting...");
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	if (TTF_Init() == -1) {
		SDL_Log("Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	CreateInterface();
	CreateGame();

	StartGameLoop();
	StartEventLoop();

	FreeBreakout();

	return EXIT_SUCCESS;

}