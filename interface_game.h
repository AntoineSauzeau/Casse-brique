#pragma once

#include "SDL/SDL.h"

void CreateGameInterface(SDL_Renderer* renderer);
void DestroyGameInterface();

void StartGameLoop();
void DrawGameInterface();

/* Messages */
void ShowStartMessage();
void HideStartMessage();
void ShowSpeed3Message();
void HideSpeed3Message();
void ShowGameoverMessage();
void HideGameoverMessage();
void ShowNextlevelMessage(int level);
void HideNextlevelMessage();
void ShowNewballMessage();
void HideNewballMessage();
void ShowPauseMessage();
void HidePauseMessage();


int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
