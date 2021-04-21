#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

#define SDL_MAIN_HANDLED
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

/*
	Prototypes.
*/
void FreeBreakout();
void ExitBreakout(int);
void ShowError(char* format, int code);




void FreeBreakout() 
{
	DestroyInterface();
	TTF_Quit();
	SDL_Quit();
}

void ExitBreakout(int code)
{
	FreeBreakout();
	exit(code);
}

void ShowError(char* format, int code) 
{
	char error_string[94] = { '\0' };
	strerror_s(error_string, 94, errno);
	fprintf(stderr, format, error_string);
}








int main(int argv, char** args) {

	printf("Starting...");
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	if (TTF_Init() == -1) {
		SDL_Log("Unable to initialize TTF : %s", TTF_GetError());
		ExitBreakout(EXIT_FAILURE);
	}

	CreateInterface();

	StartUpdateLoop();
	StartEventLoop();

	FreeBreakout();

	return EXIT_SUCCESS;

}