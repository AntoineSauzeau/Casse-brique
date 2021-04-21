#pragma once

#include "SDL/SDL.h"

#define FPS 120

void CreateGameInterface(SDL_Renderer* renderer);
void DestroyGameInterface();

void UpdateGameInterface();
void GameInterfaceEvent(SDL_Event* event);

/* Messages */
void ShowStartMessage();
void HideStartMessage();
void ShowSpeed2Message();
void HideSpeed2Message();
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

